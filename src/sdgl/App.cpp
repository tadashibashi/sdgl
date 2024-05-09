#include "App.h"

#include <sdgl/platform.h>
#include <sdgl/core/backend/Backend.h>
#include <sdgl/logging.h>
#include <angles.h>

#include <utility>

namespace sdgl {
    struct App::Impl {
        Impl(Backend *backend, string title, int width, int height, WindowInit::Flags flags,
          PluginConfig plugins) :
            backend(backend), title(std::move(title)), width(width), height(height),
            flags(flags), plugins(std::move(plugins)), window(nullptr), args(), audio(), currentTime(), lastFrameTime()
        {}

        Backend *backend;
        string title;
        int width;
        int height;
        WindowInit::Flags flags;
        PluginConfig plugins;
        Window *window;
        vector<string> args;
        AudioEngine audio;

        double currentTime, lastFrameTime;
    };

    App::App(const string &title, const int width, const int height,
             const WindowInit::Flags flags, const PluginConfig &plugins) :
        m(new Impl(new Backend, title, width, height, flags, plugins))
    {
    }

    App::~App()
    {
        delete m->backend;
        delete m;
    }

    int App::run(const int argc, char *argv[])
    {
        SDGL_LOG("Initialize SDGL App: {}", m->title);

        const auto be = m->backend;
        if (!be->init())
            return ErrorCode::BackendInitError;

        SDGL_LOG("Window Backend:  {}", be->name());

        const auto window = be->createWindow(m->title, m->width, m->height, m->flags, m->plugins);
        if (!window)
        {
            be->shutdown();
            return ErrorCode::CreateWindowFailed;
        }

        // Display vendor information
        {
            const auto vendor       = glGetString(GL_VENDOR);
            const auto renderer     = glGetString(GL_RENDERER);
            const auto shadingLang  = glGetString(GL_SHADING_LANGUAGE_VERSION);

            // Use conditionals since some platforms return nullptr
            if (vendor)
                SDGL_LOG("OpenGL Vendor:   {}", reinterpret_cast<const char *>(vendor));
            if (renderer)
                SDGL_LOG("OpenGL Renderer: {}", reinterpret_cast<const char *>(renderer));
            if (shadingLang)
                SDGL_LOG("GLSL Version:    {}", reinterpret_cast<const char *>(shadingLang));
        }


        if (!m->audio.init())
        {
            be->destroyWindow(window);
            be->shutdown();
            return ErrorCode::AudioInitError;
        }
        SDGL_LOG("Audio engine initialized.");


        m->window = window;
        m->args = vector<string>(argv, argv + argc);

        if (!init())
        {
            be->destroyWindow(window);
            be->shutdown();
            return ErrorCode::AppInitError;
        }

        SDGL_LOG("Client app initialized, entering main loop");

#ifdef SDGL_PLATFORM_EMSCRIPTEN
        SDGL_EMSCRIPTEN_MAINLOOP_BEGIN
#else
        while (!window->shouldClose())
#endif
        {
            runOneFrame();
        }
#ifdef SDGL_PLATFORM_EMSCRIPTEN
    SDGL_EMSCRIPTEN_MAINLOOP_END
#endif

        shutdown();

        m->audio.shutdown();
        be->destroyWindow(window);
        be->shutdown();

        return ErrorCode::Ok;
    }

    void App::runOneFrame()
    {
        m->backend->processInput();
        const auto window = m->window;

        m->lastFrameTime = m->currentTime;
        m->currentTime = m->backend->getAppTime();

        window->startFrame();
        update();
        m->audio.update();
        window->endFrame();

        window->makeCurrent();
        render();
        window->render(); // future: to give control over plugin render order vs app render, make call to render a
        // window render plugin with 0 priority, or modify callbacks
        // startRender / endRender like startFrame / endFrame
        window->swapBuffers();
    }

    void App::quit()
    {
        m->window->setShouldClose(true);
    }

    Window *App::window() const
    {
        return m->window;
    }

    AudioEngine *App::audio()
    {
        return &m->audio;
    }

    double App::getTime() const
    {
        return m->backend->getAppTime();
    }

    double App::getDeltaTime() const
    {
        return m->currentTime - m->lastFrameTime;
    }
}

