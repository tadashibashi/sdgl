#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#include <sdgl/logging.h>

#if SDGL_DEBUG

/// Check if an FMOD function succeeded, throws a runtime_error in debug mode if not.
/// @param statement FMOD function result
#define FMOD_CHECK(statement) do { \
    FMOD_RESULT result = (statement); \
    if (result != FMOD_OK) { \
        const string _message_ = format("FMOD error in {}, line {}: {}", fs::path(__FILE__).filename(), __LINE__, FMOD_ErrorString(result)); \
        SDGL_ERROR("{}", _message_); \
        throw std::runtime_error(_message_); \
    } \
} while(0)

#else

#define FMOD_CHECK(statement) (statement)

#endif
