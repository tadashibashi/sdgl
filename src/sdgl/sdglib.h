#pragma once

// ===== Includes for pch and defines =====

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>


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

        using std::map;

        using std::array;

        using std::vector;
        using std::set;

        template <typename T>
        using func = std::function<T>;
}
