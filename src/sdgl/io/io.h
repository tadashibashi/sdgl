#pragma once
#include <sdgl/sdglib.h>

namespace sdgl::io {

    /// Read entire file into a null-terminated buffer string
    /// @param filepath        path to the file to open, if it is a relative path, it stems from the resource directory
    /// @param outBuffer [out] buffer to receive data
    /// @returns file buffer string, or unset std::optional if file failed to load
    bool readFile(const fs::path &filepath, string *outBuffer);
    bool readFile(const fs::path &filepath, vector<ubyte> *outBuffer);

    /// Get path to read-only resource directory
    const fs::path &getResourcePath();

    /// Get path to the app data path, write / read typically allowed here
    /// Best to cache this somewhere
    fs::path getDataPath(const char *org, const char *appName);

    struct WriteMode  {
        enum Enum {
            Truncate,
            Append
        };
    };

    /// Write data buffer to output file, overwriting / truncating anything in that file, if pre-existing
    /// @param filepath path to the resource to write
    /// @param data     pointer to the data buffer to write
    /// @param length   write length of the data buffer in bytes
    /// @returns whether function succeeded - check sdgl::getError for details
    bool writeFile(const fs::path &filepath, const void *data, long length);
    /// Write data buffer to output file, overwriting / truncating anything in that file, if pre-existing
    /// @param filepath path to the resource to write
    /// @param data     the data buffer to write
    /// @returns whether function succeeded - check sdgl::getError for details
    bool writeFile(const fs::path &filepath, const string &data);
    /// Write data buffer to output file, overwriting / truncating anything in that file, if pre-existing
    /// @param filepath path to the resource to write
    /// @param data     the data buffer to write
    /// @returns whether function succeeded - check sdgl::getError for details
    bool writeFile(const fs::path &filepath, const vector<ubyte> &data);
}