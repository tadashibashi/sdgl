#pragma once
#include <sdgl/core/Window.h>
#include <sdgl/audio/AudioEngine.h>

namespace sdgl {

    /// Simple one-windowed application
    class App {
    public:
        App(
            const string &title,                           ///< Window title, and application name
            int width,                                     ///< Initial window width
            int height,                                    ///< Initial window height
            WindowInit::Flags flags = WindowInit::None,   ///< Window flags
            const PluginConfig &plugins = {.imgui=true}    ///< Window plugin configuration
        );
        virtual ~App();

        /// Execute application, passing in arguments from the command line
        /// @param argc number of command line args
        /// @param argv argument array
        /// @returns return code, equivalent to `ErrorCode::Enum`
        int run(int argc, char *argv[]);

        /// Flags the application to quit at the start of next frame
        void quit();

        /// Get the app window
        [[nodiscard]]
        Window *window() const;

        AudioEngine *audio();

        /// Get the time, in seconds, since the windowing library was initialized
        [[nodiscard]]
        double getTime() const;

        /// Get the time, in seconds, since the last call to update
        [[nodiscard]]
        double getDeltaTime() const;

        struct ErrorCode
        {
            /// Application error code returned from `App::run`
            enum Enum : int
            {
                /// No error
                Ok = 0,
                /// Backend windowing library failed during initialization
                BackendInitError,
                /// Backend windowing library failed to create the window
                CreateWindowFailed,
                /// Audio engine failed to initialize
                AudioInitError,
                /// Application subclass failed to init
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
