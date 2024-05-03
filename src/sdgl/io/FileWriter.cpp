#include "FileWriter.h"
#include <fstream>
#include <sdgl/assert.h>

#include "endian.h"

struct sdgl::io::FileWriter::Impl
{
    std::ofstream file{};
    Endian::Enum endian = Endian::Unknown;
};

sdgl::io::FileWriter::FileWriter() : m(new Impl)
{

}

sdgl::io::FileWriter::FileWriter(const fs::path &filepath, const Endian::Enum targetEndian) : FileWriter()
{
    open(filepath, targetEndian);
}

sdgl::io::FileWriter::~FileWriter()
{
    delete m;
}

bool sdgl::io::FileWriter::open(const fs::path &filepath, const Endian::Enum targetEndian)
{
    m->file.open(filepath, std::ios::binary | std::ios::out);

    if (m->file.is_open())
    {
        m->endian = targetEndian;
        return true;
    }

    return false;
}

bool sdgl::io::FileWriter::isOpen() const
{
    return m->file.is_open();
}

static std::ios_base::seekdir s_seekLocs[] = {
    std::ios_base::beg,
    std::ios_base::cur,
    std::ios_base::end,
};

bool sdgl::io::FileWriter::seek(const long position, const Location::Enum location)
{
    SDGL_ASSERT(m->file.is_open());

    return static_cast<bool>(m->file.seekp(position, s_seekLocs[location]));
}

sdgl::uint sdgl::io::FileWriter::position() const
{
    SDGL_ASSERT(m->file.is_open());

    return m->file.tellp();
}

sdgl::uint sdgl::io::FileWriter::write(const string &str, const bool appendTerminator)
{
    return write(str.data(), str.length(), appendTerminator);
}

sdgl::uint sdgl::io::FileWriter::write(const string_view str, const bool appendTerminator)
{
    return write(str.data(), str.length(), appendTerminator);
}

sdgl::uint sdgl::io::FileWriter::write(const fs::path &str, const bool appendTerminator)
{
    return write(str.native().data(), str.native().length(), appendTerminator);
}

sdgl::uint sdgl::io::FileWriter::write(const char *str, const size_t length, const bool appendTerminator)
{
    SDGL_ASSERT(m->file.is_open());
    if (length == 0) return 0;

    if (!m->file.write(str, (std::streamsize)length))
        return 0;

    if (appendTerminator)
    {
        m->file.write("\0", 1);
        return length + 1;
    }

    return length;
}

sdgl::uint sdgl::io::FileWriter::writeRaw(const void *buffer, const size_t size)
{
    SDGL_ASSERT(m->file.is_open());

    return m->file.write((char *)buffer, (std::streamsize)size) ?
               size : 0;
}

sdgl::io::Endian::Enum sdgl::io::FileWriter::endian() const
{
    return m->endian;
}

void sdgl::io::FileWriter::close()
{
    m->file.close();
}
