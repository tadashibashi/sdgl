#include "FileWatcherCache.h"

#include <sdgl/logging.h>
#include <sdgl/io/BufferView.h>
#include <sdgl/io/FileWriter.h>
#include <sdgl/io/io.h>

#include <utility>

sdgl::FileWatcherCache::FileWatcherCache(string_view id, fs::path cachePath, fs::path folderPath,
    func<void(const fs::path &filepath)> onFileUpdated, vector<string> exts)
    : m_cache(), m_cachePath(std::move(cachePath)), m_folderPath(std::move(folderPath)), m_id(id.substr(0, 4)), m_onFileUpdated(std::move(onFileUpdated)),
      m_exts(std::move(exts))
{
    // ensure id is valid
    SDGL_ASSERT(m_id.size() == 4,
        "Cache identifier string must be 4 chars");

    // ensure folder path points to an existing directory
    SDGL_ASSERT(fs::is_directory(m_folderPath),
        "Target cache file exists");

    // ensure cache directory exists, creating it if not
    if (m_cachePath.has_parent_path())
        fs::create_directories(m_cachePath.parent_path());

    // populate cache from file
    readCache();
}

sdgl::uint sdgl::FileWatcherCache::updateCache(vector<string> *outFilenames)
{
    uint numUpdated = 0;

    for (const auto &entry : fs::recursive_directory_iterator(m_folderPath))
    {
        if (!entry.is_regular_file())
            continue;

        if (!m_exts.empty())
        {
            if (std::ranges::find(m_exts, entry.path().extension().native()) == m_exts.end())
            {
                continue;
            }
        }

        // compare write times
        auto lastWriteTime = static_cast<uint64>(entry.last_write_time().time_since_epoch().count());
        const auto &entryPath = entry.path().native();

        if (auto it = m_cache.find(entryPath); it != m_cache.end()) // check cache
        {
            if (it->second < lastWriteTime) // update cache if updates to file were made
            {
                it->second = lastWriteTime;
                if (m_onFileUpdated)
                    m_onFileUpdated(it->first);
                if (outFilenames)
                    outFilenames->emplace_back(it->first.native());
                ++numUpdated;
            }
        }
        else                                                       // not in cache, create entry
        {
            m_cache[entryPath] = lastWriteTime;
            if (m_onFileUpdated)
                m_onFileUpdated(entry.path());
            if (outFilenames)
                outFilenames->emplace_back(it->first.native());
            ++numUpdated;
        }
    }

    return numUpdated;
}

bool sdgl::FileWatcherCache::readCache()
{
    if (!fs::exists(m_cachePath))
        return false;

    // Read contents of file and populate cache
    map<fs::path, uint64> cache;

    string fileBuffer;
    io::readFile(m_cachePath, &fileBuffer);

    io::BufferView view(fileBuffer);

    // Read header info
    {
        char headerChar;
        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == 'S');

        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == 'F');

        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == '$');

        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == 1);

        uint contentLength;
        SDGL_ASSERT(view.read(contentLength));
        SDGL_ASSERT(contentLength == fileBuffer.length() - 8);
    }

    uint numFiles = 0;

    // Read header block info
    {
        char headerChar;
        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == 'h');
        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == 'e');
        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == 'a');
        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == 'd');

        uint headerSize;
        SDGL_ASSERT(view.read(headerSize));
        SDGL_ASSERT(headerSize == 32);

        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == m_id[0]);
        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == m_id[1]);
        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == m_id[2]);
        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == m_id[3]);

        SDGL_ASSERT(view.read(numFiles));

        view.move(view.position() + 24);
    }

    // Read Data block
    {
        char headerChar;
        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == 'd');
        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == 'a');
        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == 't');
        SDGL_ASSERT(view.read(headerChar));
        SDGL_ASSERT(headerChar == 'a');

        uint blockSize;
        SDGL_ASSERT(view.read(blockSize));
        SDGL_ASSERT(view.size() - (view.position() + blockSize) == 0); // last block should align with file end

        // Read each file data
        for (uint i = 0; i < numFiles; ++i)
        {
            SDGL_ASSERT(view.position() < fileBuffer.size());

            string filename;
            SDGL_ASSERT(view.read(filename));

            uint64 timestamp;
            SDGL_ASSERT(view.read(timestamp));

            cache.try_emplace(filename, timestamp);
        }
    }

    // Done, commit cache
    m_cache.swap(cache);
    return true;
}

bool sdgl::FileWatcherCache::writeCache() const
{
    // Open file writer
    io::FileWriter writer;
    if (!writer.open(m_cachePath, io::Endian::Little))
    {
        return false;
    }

    // Write header info
    writer.write("SF$", false);
    writer.write(ubyte(1));

    writer.write<uint>(0); // dummy value, we'll come back to it later once file size is established

    const auto headBlockPos = writer.position();

    // Write head block
    {
        // "head"
        writer.write("head", false);

        // block length
        writer.write<uint>(32u);

        // file cache 4-char id
        SDGL_ASSERT(m_id.size() == 4);
        writer.write(m_id, false);

        writer.write<uint>(m_cache.size());

        // 24 empty reserved bytes
        static_assert(sizeof(uint64) == 8);
        writer.write<uint64>(0);
        writer.write<uint64>(0);
        writer.write<uint64>(0);
    }

    // Write data block
    {
        writer.write("data", false);
        writer.write<uint>(0); // dummy value to set once all have been set
        const auto dataBlockDataPos = writer.position();

        for (const auto &[filename, timestamp] : m_cache)
        {
            writer.write(filename);
            writer.write(timestamp);
        }

        // write data block size
        const auto dataBlockSize = writer.position() - dataBlockDataPos;
        SDGL_ASSERT(writer.seek(dataBlockDataPos - 4, io::FileWriter::Location::Beg));
        writer.write<uint>(dataBlockSize);
    }

    // write file size at beginning
    SDGL_ASSERT(writer.seek(0, io::FileWriter::Location::End));
    const auto fileSize = writer.position();
    SDGL_ASSERT(writer.seek(headBlockPos-4, io::FileWriter::Location::Beg));
    writer.write(fileSize - 8); // -8 since the first 8 bytes don't count

    return true;
}

void sdgl::FileWatcherCache::printCache() const
{
    SDGL_LOG("File cache \"{}\":", m_id);

    for (const auto &[path, timestamp] : m_cache)
    {
        SDGL_LOG("    {}: {}", path, timestamp);
    }
}
