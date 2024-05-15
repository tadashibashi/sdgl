#pragma once

#include <cstdint>

#include <AL/al.h>
#include <AL/alc.h>

namespace sdgl::audio::detail {
    bool alCheckImpl(const char *filename, const uint_fast32_t line);
    bool alcCheckImpl(const char *filename, const uint_fast32_t line, ALCdevice *device);
}

#define alCheck() (sdgl::audio::detail::alCheckImpl(__FILE__, __LINE__))
#define alcCheck(device) (sdgl::audio::detail::alcCheckImpl(__FILE__, __LINE__, device))
