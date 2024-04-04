#include "emscripten.h"

#if defined(SDGL_PLATFORM_EMSCRIPTEN)

#include <sdgl/logging.h>
#include <utility>

namespace sdgl::platform::emscripten {
    static func<void()> s_mainLoop;
    void setMainLoop(func<void()> callback)
    {
        s_mainLoop = std::move(callback);
    }
    void mainLoop()
    {
        SDGL_ASSERT(s_mainLoop);
        s_mainLoop();
    }
}

#endif
