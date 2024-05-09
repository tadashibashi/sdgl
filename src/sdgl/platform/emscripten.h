#pragma once
#include <sdgl/platform.h>

#if defined(SDGL_PLATFORM_EMSCRIPTEN)
#include <sdgl/sdglib.h>

namespace sdgl::platform::emscripten {
    void setMainLoop(func<void()> callback);
    void mainLoop();
}

#include <emscripten/emscripten.h>
#define SDGL_EMSCRIPTEN_MAINLOOP_BEGIN sdgl::platform::emscripten::setMainLoop([&]() -> void {
#define SDGL_EMSCRIPTEN_MAINLOOP_END }); emscripten_set_main_loop(sdgl::platform::emscripten::mainLoop, -1, 1);
#else

#define SDGL_EMSCRIPTEN_BEGIN_MAIN
#define SDGL_EMSCRIPTEN_END_MAIN

#endif
