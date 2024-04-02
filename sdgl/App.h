#pragma once
#include <sdgl/backends/Backend.h>

namespace sdgl {
    /**
     * Simple one-windowed application
     */
    class App {
    public:
        App(const string &title, int width, int height,
            WindowFlags::Enum flags=WindowFlags::None,
            const PluginConfig &plugins = {.imgui=true});
        virtual ~App();

        void run();

        void quit();

        [[nodiscard]]
        Window *getWindow() const;
    protected:
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void render() = 0;
        virtual void shutdown() = 0;
    private:
        void runOneFrame();
#ifdef SDGL_PLATFORM_EMSCRIPTEN
        static void emMainLoop(void *arg);
#endif
        struct Impl;
        Impl *m;
    };
}

