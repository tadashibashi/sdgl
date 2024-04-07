#pragma once

#include <sdgl/sdglib.h>
#include <sdgl/logging.h>
#include "endian.h"

namespace sdgl::io {
    struct Endian
    {
        enum Enum
        {
            Unknown,
            Little,
            Big,
        };
    };

    inline constexpr Endian::Enum SystemEndian = endian::Little ? Endian::Little :
                                                     endian::Big ? Endian::Big : Endian::Unknown;

    template <typename T>
    concept BufferReadable = std::is_arithmetic_v<T> && std::is_default_constructible_v<T>;

    /// Helper to view a memory buffer and parse values from it
    /// @warning this should only be used on known data types
    class BufferView
    {
    public:
        /// Create BufferView from a buffer
        /// @param buffer pointer to memory to view
        /// @param size size of the buffer in bytes
        /// @param endianness expected endianness of numeric data types (strings are always handled in little-endian order)
        BufferView(const void *buffer, const size_t size, const Endian::Enum endianness = Endian::Little) :
            m_buf((const ubyte *)buffer), m_pos(0), m_size(size), m_endian(endianness)
        {}

        /// Create BufferView from null-terminated c-string
        /// @param buffer c string with memory to view
        /// @param endianness expected endianness of numeric data types (strings are always handled in little-endian order)
        explicit BufferView(const char *buffer, const Endian::Enum endianness = Endian::Little) :
            m_buf((const ubyte *)buffer), m_pos(0), m_size(std::strlen(buffer)), m_endian(endianness)
        {}

        /// Create BufferView from string of byte data
        /// @param buffer string containing memory to view
        /// @param endianness expected endianness of numeric data types (string are always handled in little-endian order)
        explicit BufferView(const string &buffer, const Endian::Enum endianness = Endian::Little) :
            m_buf((const ubyte *)buffer.data()), m_pos(0), m_size(buffer.length()), m_endian(endianness)
        {}

        /// Create BufferView from vector of ubyte
        /// @param buffer vector containing memory to view
        /// @param endianness expected endianness of numeric data types (strings are always handled in little-endian order)
        explicit BufferView(const vector<ubyte> &buffer, const Endian::Enum endianness = Endian::Little) :
            m_buf(buffer.data()), m_pos(0), m_size(buffer.size()), m_endian(endianness)
        {}

        /// Create BufferView from vector of bytes
        /// @param buffer vector containing memory to view
        /// @param endianness expected endianness of numeric data types (strings are always handled in little-endian order)
        explicit BufferView(const vector<char> &buffer, const Endian::Enum endianness = Endian::Little) :
            m_buf((const ubyte *)buffer.data()), m_pos(0), m_size(buffer.size()), m_endian(endianness)
        {}

        /// Read data into a numeric value. Currently, this function only supports primitive numeric types.
        /// Use other overloads for strings.
        /// @tparam T primitive numberic type - please ensure this type's size is what you intend to read.
        /// @param outVal [out] value to receive the data read
        /// @returns number of bytes read; if > 0, `outVal` will contain the read data;
        ///          if 0, `outVal` will be unmodified. Check sdgl::getError on 0 for more details.
        template <BufferReadable T>
        uint read(T &outVal)
        {
            constexpr auto size = sizeof(T);
            if (m_pos + size > m_size)
            {
                SDGL_ERROR("Size of read exceeds buffer size");
                return 0;
            }

            std::memcpy((void *)&outVal, m_buf + m_pos, size);

            if (size > 1 && SystemEndian != m_endian) // swap needed
            {
                outVal = endian::swap(outVal);
            }

            m_pos += size;
            return size;
        }

        template <BufferReadable T>
        T read()
        {
            T t;
            SDGL_ASSERT(read(t));
            return t;
        }

        /// Read a null-terminated string from the buffer
        /// @param outString [out] string to receive the data
        /// @param maxSize         maximum number of characters to fill `outString`; the buffer position will still read
        ///                        until the null-terminator, even if it exceeds this value, but such characters will
        ///                        not be added to `outString`.
        ///
        /// @returns the number of bytes read (including null terminator) -
        ///          if > 0 `outString` will contain data, otherwise `outString` will be unmodified.
        ///          Check sdgl::getError() for more information on 0.
        ///          Note: this number may differ from string length + 1 if `maxSize` clipped the out value.
        uint read(string &outString, size_t maxSize = SIZE_T_MAX);

        /// Read a string with an expected length
        /// @param outString [out] string to receive the data
        /// @param length          length of the string in bytes (not including null terminator)
        uint readFixedString(string &outString, size_t length);

        /// Read a null-terminated string from the buffer
        /// @param outBuffer [out] pointer to a string buffer to receive the data
        /// @param maxSize         max length of `outBuffer`; the buffer position will still read until the
        ///                        null-terminator, even if it exceeds this value, but such characters will not be added
        ///                        to `outBuffer`.
        ///
        /// @returns the number of bytes read (including null terminator) -
        ///          if > 0 `outBuffer` will contain data, otherwise `outBuffer` will be unmodified.
        ///          Check sdgl::getError for more info on 0.
        ///          Note: this number may differ from string length + 1 if `maxSize` clipped the out value.
        uint read(char *outBuffer, size_t maxSize);

        /// Peek relative to the current location. In debug mode, an assertion is made to check bounds.
        /// @param offset - offset bytes, may be negative
        ///
        /// @returns value of byte at location
        [[nodiscard]]
        ubyte peek(int offset = 0) const;

        /// Move the view's position back to 0
        void reset()
        {
            m_pos = 0;
        }

        /// Get the buffer size
        [[nodiscard]]
        auto size() const { return m_size; }

        /// Get the number of bytes left to read (from current position to the end of the buffer)
        [[nodiscard]]
        auto bytesLeft() const { return m_size - m_pos; }

        /// Get the view's byte position
        [[nodiscard]]
        auto position() const { return m_pos; }

        /// Get the pointer to the data (at position 0)
        [[nodiscard]]
        const void *data() const { return m_buf; }

        /// Get the expected endianness that was set in the view's constructor
        [[nodiscard]]
        auto expectedEndian() const { return m_endian; }

        void move(size_t offset)
        {
            m_pos = offset;
        }
    private:
        const ubyte *m_buf;
        size_t m_pos;
        size_t m_size;
        Endian::Enum m_endian;
    };
}
