#pragma once

#if (defined(__APPLE__) && __clang_major__ >= 15) || !defined(__APPLE__) && __cplusplus >= 202002L
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
#include <spdlog/fmt/bundled/format.h>
namespace sdgl {
    template <typename... T>
    FMT_NODISCARD FMT_INLINE auto format(const fmt::format_string<T...> &format,
        T&&... args) -> std::string {
        return fmt::vformat(format, fmt::make_format_args(args...));
    }
}
#endif

