#include "io.h"

#include <sdgl/assert.h>
#include <sdgl/logging.h>

#include <SDL_filesystem.h>

#include <fstream>

namespace sdgl::io {

    bool readFile(const fs::path &filepath, string *outBuffer)
    {
        SDGL_ASSERT(outBuffer, "Out buffer should not be null");

        const auto fullpath = (filepath.is_absolute()) ? filepath : (getResourcePath() / filepath);

        std::ifstream file(fullpath, std::ios::binary | std::ios::in);
        if (!file.is_open())
        {
            SDGL_ERROR("Failed to open file \"{}\": {}", fullpath, std::strerror(errno));
            return false;
        }

        // Get the file size
        file.seekg(0, std::ios::end);
        auto fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        //fileSize -= file.tellg(); // disregard file header metadata

        // Read file into out buffer
        string temp(fileSize, ' ');

        if (!file.read(temp.data(), fileSize))
        {
            SDGL_ERROR("Failure while reading file \"{}\": {}", filepath, std::strerror(errno));
            file.close();

            return false;
        }

        outBuffer->swap(temp);
        return true;
    }

    bool readFile(const fs::path &filepath, vector<ubyte> *outBuffer)
    {
        SDGL_ASSERT(outBuffer, "Out buffer should not be null");

        const auto fullpath = (filepath.is_absolute()) ? filepath : (getResourcePath() / filepath);

        std::ifstream file(fullpath, std::ios::binary | std::ios::in);
        if (!file.is_open())
        {
            SDGL_ERROR("Failed to open file \"{}\": {}", fullpath, std::strerror(errno));
            return false;
        }

        // Get the file size
        file.seekg(0, std::ios::end);
        auto fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        //fileSize -= file.tellg(); // disregard file header metadata

        // Read file into out buffer
        vector<ubyte> temp(fileSize, 0);

        if (!file.read((char *)temp.data(), fileSize))
        {
            SDGL_ERROR("Failure while reading file \"{}\": {}", filepath, std::strerror(errno));
            file.close();

            return false;
        }

        outBuffer->swap(temp);
        return true;
    }

    const fs::path &getResourcePath()
    {
        static fs::path resourcePath;
        if (resourcePath.empty())
        {
            auto basePath = SDL_GetBasePath();
            if (basePath)
            {
                resourcePath.assign(basePath);
                SDL_free(basePath);
            }
        }

        return resourcePath;
    }

    fs::path getDataPath(const char *org, const char *appName)
    {
        fs::path dataPath;
        auto sdlPath = SDL_GetPrefPath(org, appName);
        dataPath.assign(sdlPath);
        SDL_free(sdlPath);

        return dataPath;
    }

    bool writeFile(const fs::path &filepath, const void *data, const long length)
    {
        std::ofstream file;

        file.open(filepath.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
        if (!file.is_open())
        {
            SDGL_ERROR("Failed to write file \"{}\": {}", filepath, std::strerror(errno));
            return false;
        }

        if (!file.write((const char *)data, length))
        {
            SDGL_ERROR("Failure while writing file \"{}\": {}", filepath, std::strerror(errno));
            return false;
        }

        file.close();
        return true;
    }

    bool writeFile(const fs::path &filepath, const string &data)
    {
        return writeFile(filepath, data.c_str(), (long)data.length());
    }

    bool writeFile(const fs::path &filepath, const vector<ubyte> &data)
    {
        return writeFile(filepath, (const char *)data.data(), (long)data.size());
    }
}
