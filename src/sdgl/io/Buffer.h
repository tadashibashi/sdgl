#pragma once
#include <sdgl/sdglib.h>

namespace sdgl::io {
    /// Null-terminated buffer of data used for file io
    class Buffer {
    public:
        Buffer(void *buffer, size_t size);
        Buffer(const Buffer &other);
        Buffer(Buffer &&buffer) noexcept;
        ~Buffer();

        Buffer &operator=(const Buffer &other);
        Buffer &operator=(Buffer &&other) noexcept;

        [[nodiscard]]
        const char *data() const;

        [[nodiscard]]
        size_t size() const;

        [[nodiscard]]
        bool empty() const;

    private:
        char *m_buf;
        size_t m_size;
    };
}

