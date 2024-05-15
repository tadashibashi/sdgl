#pragma once

#include <sdgl/sdglib.h>
#include <sdgl/format.h>

namespace sdgl::logging::detail {
    void setLastErrorMessage(const string &message);
    const string &getLastErrorMessage();
}

// provide for convenience to end user
namespace sdgl {
    inline const string &getError() { return logging::detail::getLastErrorMessage(); }
}

// Set logging mode if it wasn't provided by user
#ifndef SDGL_LOGGING
#   if SDGL_DEBUG
#       define SDGL_LOGGING 1
#   else
#       define SDGL_LOGGING 0
#   endif
#endif

// Define logging macros
#if SDGL_LOGGING
#   include <spdlog/logger.h>
    namespace sdgl::logging::detail {
        spdlog::logger *getCoreLogger();
        spdlog::logger *getClientLogger();
    }

/// Internal error log (or noop in release mode)
/// @param format std::format-compliant format string
/// @param ...    formattable arguments
#   define SDGL_ERROR(...) do { \
        auto message = format(__VA_ARGS__); \
        sdgl::logging::detail::setLastErrorMessage(message); \
        sdgl::logging::detail::getCoreLogger()->error(message); \
        } while(0)

/// Internal info log (or noop in release mode)
/// @param format std::format-compliant format string
/// @param ...    formattable arguments
#   define SDGL_LOG(...) (sdgl::logging::detail::getCoreLogger()->info(__VA_ARGS__))

/// Internal warning log (or noop in release mode)
/// @param format std::format-compliant format string
/// @param ...    formattable arguments
#   define SDGL_WARN(...) (sdgl::logging::detail::getCoreLogger()->warn(__VA_ARGS__))

/// Client info log (or noop in release mode)
/// @param format std::format-compliant format string
/// @param ...    formattable arguments
#   define DEBUG_LOG(...) (sdgl::logging::detail::getClientLogger()->info(__VA_ARGS__))

/// Client warning log (or noop in release mode)
/// @param format std::format-compliant format string
/// @param ...    formattable arguments
#   define DEBUG_WARN(...) (sdgl::logging::detail::getClientLogger()->warn(__VA_ARGS__))

/// Client error log (or noop in release mode)
/// @param format std::format-compliant format string
/// @param ...    formattable arguments
#   define DEBUG_ERROR(...) (sdgl::logging::detail::getClientLogger()->error(__VA_ARGS__))

#else
#   if defined(_MSC_VER)
#       define SDGL_LOG(...) __noop
#       define SDGL_WARN(...) __noop
#       define SDGL_ERROR(...) sdgl::logging::detail::setLastErrorMessage(format(__VA_ARGS__))
#       define DEBUG_LOG(...) __noop
#       define DEBUG_WARN(...) __noop
#       define DEBUG_ERROR(...) __noop
#   else
#       define SDGL_LOG(...) do {} while(0)
#       define SDGL_WARN(...) do {} while(0)
#       define SDGL_ERROR(...) sdgl::logging::detail::setLastErrorMessage(format(__VA_ARGS__))
#       define DEBUG_LOG(...) do {} while(0)
#       define DEBUG_WARN(...) do {} while(0)
#       define DEBUG_ERROR(...) do {} while(0)
#   endif
#endif

// Some common formatter impls, maybe move this to another file?
template <>
struct formatter<sdgl::fs::path>
{
    template <class FormatContext>
    constexpr auto parse(FormatContext& ctx) {
        return ctx.begin();
    }

    template <class FormatContext>
    auto format(const sdgl::fs::path& obj, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "{}", obj.native());
    }
};