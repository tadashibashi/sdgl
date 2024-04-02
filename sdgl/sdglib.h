#pragma once

// ===== Target Conditionals =====

#if __APPLE__
#   include <TargetConditionals.h>
#   define SDGL_PLATFORM_APPLE
#   if TARGET_OS_IOS
#       define SDGL_PLATFORM_IOS
#   elif TARGET_OS_VISION
        define SDG_PLATFORM_VISION
#   elif TARGET_OS_MAC
#       define SDGL_PLATFORM_MAC
#   else
#       error "Apple platform not supported"
#   endif
#elif defined(__MSC_VER)
#   define SDGL_PLATFORM_WINDOWS
#   ifndef _WIN64
#       error "Windows 32-bit builds not supported"
#   endif
#elif defined(__linux__)
#   define SDGL_PLATFORM_LINUX
#elif defined(__ANDROID__)
#   define SDGL_PLATFORM_ANDROID
#elif defined(__EMSCRIPTEN__)
#   define SDGL_PLATFORM_EMSCRIPTEN
#endif

// ===== Includes for pch and defines =====

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include <map>
//#include <glm/glm.hpp>


namespace sdgl {
        using byte = int8_t;
        using ubyte = uint8_t;
        using ushort = uint16_t;
        using uint = uint32_t;
        using uint64 = uint64_t;
        using int64 = int64_t;
        using size = size_t;
        //using vec2 = glm::vec2;

        // stl
        using string = std::string;
        using string_view = std::string_view;

        template <typename K, typename V, typename Compare = std::less<K>,
            typename Allocator = std::allocator<std::pair<const K, V> >>
        using map = std::map<K, V, Compare, Allocator>;

        template <typename T, typename Allocator = std::allocator<T>>
        using vector = std::vector<T>;

        template <typename T>
        using func = std::function<T>;
}


