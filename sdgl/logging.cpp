#if defined(SDGL_DEBUG)
#include "logging.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace sdgl::logging::detail {

#if !defined(SDGL_CLIENT_LOGGING_SILENT_MODE)
    spdlog::logger *getClientLogger()
    {
        static std::shared_ptr<spdlog::logger> s_clientLogger;
        if (!s_clientLogger)
        {
            s_clientLogger = spdlog::stdout_color_mt("debug");
            s_clientLogger->set_level(spdlog::level::trace);
            s_clientLogger->set_pattern("(%T) [%^%n%$]: %v");
        }

        return s_clientLogger.get();
    }
#endif

#if !defined(SDGL_LOGGING_SILENT_MODE)
    spdlog::logger *getCoreLogger()
    {
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        if (!s_coreLogger)
        {
            s_coreLogger = spdlog::stdout_color_mt("sdgl");
            s_coreLogger->set_level(spdlog::level::trace);
            s_coreLogger->set_pattern("(%T) [%^%n%$]: %v");

        }

        return s_coreLogger.get();
    }
#endif
}

#endif

namespace sdgl::logging::detail {
    static string s_lastCoreError;
    void setLastErrorMessage(const string &message) { s_lastCoreError = message; }
    const string &getLastErrorMessage() { return s_lastCoreError; }
}


