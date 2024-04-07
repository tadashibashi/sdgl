#include "BufferView.h"

namespace sdgl::io {
    uint BufferView::read(string &outString, const size_t maxSize)
    {
        // Current position already finished reading?
        if (m_pos >= m_size)
        {
            SDGL_ERROR("Cannot read string from buffer because BufferView is done reading");
            return 0;
        }

        try
        {
            // find the null terminator's position (end of the string)
            size_t termPos = m_pos;
            while (termPos < m_size && termPos - m_pos < maxSize && m_buf[termPos] != 0)
            {
                ++termPos;
            }

            // copy the data
            outString = string(m_buf + m_pos, m_buf + termPos);

            if (termPos < m_size && m_buf[termPos] != 0) // last position not a terminator from early clipping from maxSize?
            {
                // finish until end of the string
                while(termPos < m_size && m_buf[termPos] != 0)
                    ++termPos;
            }

            m_pos = termPos + 1;
            return outString.size() + 1; // +1 includes the null-terminator
        }
        catch(const std::exception &e)
        {
            SDGL_ERROR("Failed to read string from buffer: {}", e.what());
            return 0;
        }
        catch(...)
        {
            SDGL_ERROR("Failed to read string from buffer: unknown error");
            throw;
        }
    }

    uint BufferView::readFixedString(string &outString, size_t length)
    {
        if (m_pos + length > m_size)
        {
            SDGL_ERROR("Size of read exceeds buffer size");
            return 0;
        }

        outString = string(m_buf + m_pos, m_buf + m_pos + length);
        m_pos += length + 1; // +1 to move one past null terminator
        return length + 1;
    }

    uint BufferView::read(char *outBuffer, const size_t maxSize)
    {
        // Current position already finished reading?
        if (m_pos >= m_size)
        {
            SDGL_ERROR("Cannot read string from buffer because BufferView is done reading");
            return 0;
        }

        try
        {
            // find the null terminator's position
            size_t termPos = m_pos;
            while (termPos < m_size &&
                termPos - m_pos < maxSize - 1 &&  // must be `maxSize-1` to leave room for null terminator
                m_buf[termPos] != 0)
            {
                ++termPos;
            }

            std::memcpy(outBuffer, m_buf + m_pos, termPos - m_pos);
            outBuffer[termPos - m_pos] = 0; // set terminator in the output string

            if (termPos < m_size && m_buf[termPos] != 0) // last index not a null terminator due to early break from clipping?
            {
                while (termPos < m_size && m_buf[termPos] != 0) // finish until end of string
                    ++termPos;
            }

            auto length = termPos - m_pos + 1; // + 1 includes the null terminator
            m_pos = termPos + 1;
            return length;
        }
        catch(const std::exception &e)
        {
            SDGL_ERROR("Failed to read string from buffer: {}", e.what());
            return 0;
        }
        catch(...)
        {
            SDGL_ERROR("Failed to read string from buffer: unknown error");
            throw;
        }
    }

    ubyte BufferView::peek(const int offset) const
    {
        const int index = static_cast<int>(m_pos) + offset;

        SDGL_ASSERT(index >= 0 && index < m_size);

        return m_buf[index];
    }
}
