#pragma once
#include <sdgl/sdglib.h>

#include "endian.h"

namespace sdgl::io {
    /// Simple wrapper around an ofstream, but it flips bits in case the current operating system endianness differs
    /// from the target endianness of the output.
    /// @note all files at target location are deleted when FileWriter opens output stream, so please make sure to
    ///       backup data that may be needed.
    class FileWriter {
    public:
        struct Location
        {
            enum Enum
            {
                Beg,
                Cur,
                End,
            };
        };

        FileWriter();
        explicit FileWriter(const fs::path &filepath, Endian::Enum targetEndian = Endian::Little);
        ~FileWriter();

        /// @param filepath path to the file to write
        /// @param targetEndian target endianness to write to (strings are always little endian)
        bool open(const fs::path &filepath, Endian::Enum targetEndian = Endian::Little);

        /// Check if output file was successfully opened
        [[nodiscard]]
        bool isOpen() const;

        /// Write primitive type to the buffer
        template <typename T>
        uint write(const T &value)
        {
            if (endian() != SystemEndian)
            {
                auto reversed = endian::swap(value);
                return writeRaw(&reversed, sizeof(T));
            }

            return writeRaw(&value, sizeof(T));
        }

        /// Seek to a position in the file to write to
        /// @param position byte position relative to location
        /// @param location location to seek from
        /// @returns whether seek succeeded
        bool seek(long position, Location::Enum location = Location::Cur);

        [[nodiscard]]
        uint position() const;

        /// Write string to the file
        /// @param str string to write
        /// @param appendTerminator whether to add a null terminator after the end of the string
        /// @returns number of bytes written
        uint write(const string &str, bool appendTerminator = true);

        /// Write string to the file
        /// @param str string to write
        /// @param appendTerminator whether to add a null terminator after the end of the string
        /// @returns number of bytes written
        uint write(string_view str, bool appendTerminator = true);

        uint write(const fs::path &str, bool appendTerminator = true);

        uint write(const char *str, bool appendTerminator = true)
        {
            return write(string_view(str), appendTerminator);
        }

        /// Write string to the file
        /// @param str string to write
        /// @param length length of the string (not including null terminator)
        /// @param appendTerminator whether to add a null terminator after the end of the string
        /// @returns number of bytes written
        uint write(const char *str, size_t length, bool appendTerminator = true);

        /// Write raw bytes to the file
        uint writeRaw(const void *buffer, size_t size);

        [[nodiscard]]
        Endian::Enum endian() const;

        void close();
    private:
        struct Impl;
        Impl *m;
    };

}
