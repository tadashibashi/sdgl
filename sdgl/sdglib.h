#pragma once

// ===== Includes for pch and defines =====

#include <cstdint>
#include <filesystem>
#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include <map>

namespace sdgl {
        using byte = int8_t;
        using ubyte = uint8_t;
        using int16 = int16_t;
        using uint16 = uint16_t;
        using ushort = uint16_t;
        using uint = uint32_t;
        using uint64 = uint64_t;
        using int64 = int64_t;
        using size_t = size_t;

        // stl
        using string = std::string;
        using string_view = std::string_view;

        namespace fs = std::filesystem;

        template <typename K, typename V, typename Compare = std::less<K>,
            typename Allocator = std::allocator<std::pair<const K, V> >>
        using map = std::map<K, V, Compare, Allocator>;

        template <typename T, typename Allocator = std::allocator<T>>
        using vector = std::vector<T>;

        template <typename T>
        using func = std::function<T>;
}
