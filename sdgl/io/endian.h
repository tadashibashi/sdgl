#pragma once

#include <../sdglib.h>
#include <bit>

namespace sdgl::io::endian {
    inline constexpr bool Little = std::endian::native == std::endian::little;
    inline constexpr bool Big = std::endian::native == std::endian::big;
    inline constexpr bool Middle = !Little && !Big;

    template <typename T>
    T swap(T u)
    {
        union
        {
            T u;
            ubyte u8[sizeof(T)];
        } source, dest;

        source.u = u;

        for (size_t k = 0; k < sizeof(T); k++)
            dest.u8[k] = source.u8[sizeof(T) - k - 1];

        return dest.u;
    }
}
