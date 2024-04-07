#pragma once
#include <sdgl/Window.h>

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

        int run(int argc, char *argv[]);

        void quit();

        [[nodiscard]]
        Window *getWindow() const;

        struct ErrorCode
        {
            enum Enum : int
            {
                Ok = 0,
                BackendInitError,
                CreateWindowFailed,
                AppInitError,
            };
        };
    protected:
        virtual bool init() = 0;
        virtual void update() = 0;
        virtual void render() = 0;
        virtual void shutdown() = 0;
    private:
        void runOneFrame();
        struct Impl;
        Impl *m;
    };
}

