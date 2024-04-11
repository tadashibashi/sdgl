#include "io.h"
#include <fstream>

#include "../logging.h"

namespace sdgl::io {

    bool readFile(const string &filepath, string *outBuffer)
    {
        SDGL_ASSERT(outBuffer, "Out buffer should not be null");

        std::ifstream file(filepath, std::ios::binary | std::ios::in);
        if (!file.is_open())
        {
            SDGL_ERROR("Failed to open file \"{}\": {}", filepath, std::strerror(errno));
            return false;
        }

        // Get the file size
        file.seekg(0, std::ios::end);
        uint fileSize = file.tellg();
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

    bool readFile(const string &filepath, vector<ubyte> *outBuffer)
    {
        SDGL_ASSERT(outBuffer, "Out buffer should not be null");

        std::ifstream file(filepath, std::ios::binary | std::ios::in);
        if (!file.is_open())
        {
            SDGL_ERROR("Failed to open file \"{}\": {}", filepath, std::strerror(errno));
            return false;
        }

        // Get the file size
        file.seekg(0, std::ios::end);
        uint fileSize = file.tellg();
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

    bool writeFile(const string &filepath, const void *data, const long length)
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

    bool writeFile(const string &filepath, const string &data)
    {
        return writeFile(filepath, data.c_str(), (long)data.length());
    }

    bool writeFile(const string &filepath, const vector<ubyte> &data)
    {
        return writeFile(filepath, (const char *)data.data(), (long)data.size());
    }
}
