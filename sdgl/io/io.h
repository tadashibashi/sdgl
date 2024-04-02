#pragma once
#include <sdgl/sdglib.h>

namespace sdgl::io {

    /**
     * Read entire file into a null-terminated buffer string
     * @param filepath - path to the file to open
     * @returns file buffer string, or unset std::optional if file failed to load
     */
    bool readFile(const string &filepath, string *outBuffer);

    struct WriteMode  {
        enum Enum {
            Truncate,
            Append
        };
    };

    bool writeFile(const string &filepath, const string& data, WriteMode::Enum mode = WriteMode::Truncate);
    bool writeFile(const string &filepath, const vector<byte>& data, WriteMode::Enum mode = WriteMode::Truncate);
    bool writeFile(const string &filepath, const vector<ubyte>& data, WriteMode::Enum mode = WriteMode::Truncate);

    /**
     * Load image file into a gpu texture
     * @return texture id or 0, if any errors (check logs)
     */
    uint loadTexture(const string &filepath);
    uint loadTextureMem(const string &bytes);

}