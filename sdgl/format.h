#pragma once

#if defined(__APPLE__)

#if __clang_major__ >= 15
#include <format>

namespace sdgl {
    template <typename... T>
    [[nodiscard]] inline
    auto format(const std::string_view format,
        T&&... args) -> std::string {
        return std::vformat(format, std::make_format_args(args...));
    }
}
#else
#include <spdlog/fmt/fmt.h>
namespace sdgl {
    template <typename... T>
    FMT_NODISCARD FMT_INLINE auto format(const fmt::format_string<T...> &format,
        T&&... args) -> std::string {
        return fmt::vformat(format, fmt::make_format_args(args...));
    }
}
#endif

#endif
