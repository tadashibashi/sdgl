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

#ifdef SDGL_PLATFORM_EMSCRIPTEN
#   include <sdgl/platform/emscripten.h>
#endif
