#pragma once

#if (defined(__APPLE__) && __clang_major__ >= 15) || !defined(__APPLE__) && __cplusplus >= 202002L
#include <format>
using std::formatter;
using std::format;
#else
#include <spdlog/fmt/bundled/format.h>
namespace sdgl {
    using std::formatter;
    using std::format;
}
#endif

