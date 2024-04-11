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

#if SDGL_DEBUG // ===========================================================================

#if !defined(SDGL_LOGGING_SILENT_MODE) || !defined(SDGL_CLIENT_LOGGING_SILENT_MODE)
#include <spdlog/logger.h>

#endif

#include <cassert>

#define SDGL_GET_ERROR() sdgl::logging::getLastDebugMessage()

#define IMPL_SDGL_ASSERT_1(condition) (assert(condition))
#define IMPL_SDGL_ASSERT_2(condition, comment) (assert((condition) && comment))

#define IMPL_SDGL_GET_ASSERT_MACRO(_1, _2, NAME, ...) NAME
#define SDGL_ASSERT(...) IMPL_SDGL_GET_ASSERT_MACRO(__VA_ARGS__, IMPL_SDGL_ASSERT_2, IMPL_SDGL_ASSERT_1)(__VA_ARGS__)

#ifdef SDGL_LOGGING_SILENT_MODE // turn off core logger even when in debug mode, only allow error cache to be written to
#   define SDGL_ERROR(...) sdgl::logging::setLastErrorMessage(format(__VA_ARGS__))
#   if defined(_MSC_VER)
#       define SDGL_WARN(...) __noop
#       define SDGL_LOG(...) __noop
#   else
#       define SDGL_WARN(...)
#       define SDGL_LOG(...)
#   endif
#else                                // allow all core logs
namespace sdgl::logging::detail {
    spdlog::logger *getCoreLogger();
}
#   define SDGL_ERROR(...) do { \
    auto message = format(__VA_ARGS__); \
    sdgl::logging::detail::setLastErrorMessage(message); \
    sdgl::logging::detail::getCoreLogger()->error(message); \
} while(0)
#   define SDGL_LOG(...) (sdgl::logging::detail::getCoreLogger()->info(__VA_ARGS__))
#   define SDGL_WARN(...) (sdgl::logging::detail::getCoreLogger()->warn(__VA_ARGS__))
#endif

#ifdef SDGL_CLIENT_LOGGING_SILENT_MODE // turn off client logger even when in debug mode
#   if defined(_MSC_VER)
#       define DEBUG_WARN(...) __noop
#       define DEBUG_LOG(...) __noop
#       define DEBUG_ERROR(...) __noop
#   else
#       define SDGL_WARN(...)
#       define SDGL_LOG(...)
#       define SDGL_ERROR(...)
#   endif
#else
namespace sdgl::logging::detail {
    spdlog::logger *getClientLogger();
}
#   define DEBUG_LOG(...) (sdgl::logging::detail::getClientLogger()->info(__VA_ARGS__))
#   define DEBUG_WARN(...) (sdgl::logging::detail::getClientLogger()->warn(__VA_ARGS__))
#   define DEBUG_ERROR(...) (sdgl::logging::detail::getClientLogger()->error(__VA_ARGS__))
#endif


#else // ===== non-debug mode => remove all logs ==================================================
#if defined(_MSC_VER)
#   define SDGL_LOG(...) __noop
#   define SDGL_WARN(...) __noop
#   define SDGL_ERROR(...) sdgl::logging::detail::setLastErrorMessage(format(__VA_ARGS__))
#   define SDGL_GET_ERROR() sdgl::logging::detail::getLastErrorMessage()
#   define SDGL_ASSERT(condition) __noop
#   define DEBUG_LOG(...) __noop
#   define DEBUG_WARN(...) __noop
#   define DEBUG_ERROR(...) __noop
#else
#   define SDGL_LOG(...)
#   define SDGL_WARN(...)
#   define SDGL_ERROR(...) sdgl::logging::detail::setLastErrorMessage(format(__VA_ARGS__))
#   define SDGL_GET_ERROR() sdgl::logging::detail::getLastErrorMessage()
#   define SDGL_ASSERT(...)
#   define DEBUG_LOG(...)
#   define DEBUG_WARN(...)
#   define DEBUG_ERROR(...)
#endif

#endif // =========================================================================================