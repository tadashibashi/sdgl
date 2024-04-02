#include "io.h"
#include <fstream>

#include "../logging.h"

namespace sdgl::io {
    bool readFile(const string &filepath, string *outBuffer)
    {
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
        fileSize -= file.tellg(); // disregard file header metadata

        // Read file into out buffer
        string temp(fileSize, ' ');

        if (!file.read(temp.data(), fileSize))
        {
            SDGL_ERROR("Failure while reading file '{}': {}", filepath, std::strerror(errno));
            file.close();

            return false;
        }

        outBuffer->swap(temp);
        return true;
    }
}
