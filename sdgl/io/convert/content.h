#pragma once
#include <sdgl/sdglib.h>

namespace sdgl::io::convert {

    /// SBC (SDGL Binary Content) Format v1
    /// Purpose of this format is to strip out any unnecessary header data, converted to a format that can be directly
    /// used by the engine.
    /// All integers are little endian, and unsigned unless otherwise noted. Strings are little endian.
    ///
    /// Format:
    /// 3 bytes "SBC" - file format
    /// 1 byte  - version = 1
    /// 3 bytes - sub file format type e.g. "IMG",
    /// 1 byte  - sub file format version = e.g. 1
    /// uint32  - size of the rest of the file in bytes (filesize - 8)
    ///
    /// Rest of the file consists of a series of chunks which are defined depending on the sub file format type.
    /// A chunk consists of:
    /// 4 byte string - chunk name (case-sensitive)
    /// uint32 - length of rest of chunk in bytes (adding this many bytes from here will jump to the next chunk)
    /// data filling the bytes specified in the length

    /// .SB IMG format v1
    /// header string "IMG", version 1
    /// [size]
    /// length = 8
    /// uint32 - width in pixels
    /// uint32 - height in pixels
    /// [data]
    /// length = variable
    /// series of uint32 indicating RGBA value for a pixel
    bool writeImageToSbc(const string &imageData, const string &filepath);

}