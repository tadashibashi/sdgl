#include "BitFlags.h"
#include <cstdlib>
#include <cstring>
#include <cmath>

sdgl::BitFlags::BitFlags(size_t numFlags) : m_flags(), m_size( std::ceil( numFlags / 32.f) )
{
    auto memSize = m_size * sizeof(uint32_t);
    m_flags = static_cast<uint32_t *>(std::malloc(memSize));
    std::memset(m_flags, 0, memSize);
}

sdgl::BitFlags::BitFlags(const BitFlags &other) :
    m_flags(static_cast<uint32_t *>(std::malloc(other.m_size * sizeof(uint32_t)))),
    m_size(other.m_size)
{
    std::memcpy(m_flags, other.m_flags, m_size * sizeof(uint32_t));
}

sdgl::BitFlags::BitFlags(BitFlags &&other) noexcept : m_flags(other.m_flags), m_size(other.m_size)
{
    other.m_flags = nullptr;
    other.m_size = 0;
}

sdgl::BitFlags &
sdgl::BitFlags::operator=(const BitFlags &other)
{
    if (m_flags)
        std::free(m_flags);

    m_flags = static_cast<uint32_t *>(std::malloc(other.m_size * sizeof(uint32_t)));
    m_size = other.m_size;

    std::memcpy(m_flags, other.m_flags, other.m_size * sizeof(uint32_t));
    return *this;
}

sdgl::BitFlags &
sdgl::BitFlags::operator=(BitFlags &&other) noexcept
{
    if (m_flags)
        std::free(m_flags);

    m_flags = other.m_flags;
    m_size = other.m_size;

    other.m_flags = nullptr;
    other.m_size = 0;
    return *this;
}

sdgl::BitFlags::~BitFlags()
{
    std::free(m_flags);
}

bool sdgl::BitFlags::get(size_t index) const
{
    return m_flags[index / 32u] & (1u << (index % 32u));
}

void sdgl::BitFlags::set(size_t index, bool value)
{
    auto &chunk = m_flags[index / 32u];
    auto bit = index % 32u;

    chunk = value ? chunk | (1u << bit) :
        chunk & ~(1 << bit);
}

void sdgl::BitFlags::unsetAll()
{
    std::memset(m_flags, 0, m_size * sizeof(uint32_t));
}
