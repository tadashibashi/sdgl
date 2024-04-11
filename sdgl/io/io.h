#pragma once
#include <sdgl/sdglib.h>

namespace sdgl::io {

    /// Read entire file into a null-terminated buffer string
    /// @param filepath        path to the file to open
    /// @param outBuffer [out] buffer to receive data
    /// @returns file buffer string, or unset std::optional if file failed to load
    bool readFile(const string &filepath, string *outBuffer);
    bool readFile(const string &filepath, vector<ubyte> *outBuffer);

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
    bool writeFile(const string &filepath, const void *data, long length);
    /// Write data buffer to output file, overwriting / truncating anything in that file, if pre-existing
    /// @param filepath path to the resource to write
    /// @param data     the data buffer to write
    /// @returns whether function succeeded - check sdgl::getError for details
    bool writeFile(const string &filepath, const string &data);
    /// Write data buffer to output file, overwriting / truncating anything in that file, if pre-existing
    /// @param filepath path to the resource to write
    /// @param data     the data buffer to write
    /// @returns whether function succeeded - check sdgl::getError for details
    bool writeFile(const string &filepath, const vector<ubyte> &data);
}