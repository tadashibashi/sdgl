#pragma once
#include <cstdint>
#include <cstddef>

namespace sdgl {

    /// Container for packing a bunch of boolean flags together
    class BitFlags {
    public:
        explicit BitFlags(size_t numFlags);
        BitFlags(const BitFlags &other);
        BitFlags(BitFlags &&other);
        BitFlags &operator=(const BitFlags &other);
        BitFlags &operator=(BitFlags &&other);

        ~BitFlags();

        [[nodiscard]]
        bool get(size_t index) const;

        void set(size_t index, bool value);

        const uint32_t *data() const { return m_flags; }

        /// @return number of uint32_t blocks of memory used for the flags
        const size_t size() const { return m_size; }

        void unsetAll();
    private:
        uint32_t *m_flags;
        uint32_t m_size;
    };
}