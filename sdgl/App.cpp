#include "App.h"

#include <sdgl/backends/Backend.h>
#include <sdgl/logging.h>
#include <sdgl/platform.h>

#include <sdgl/gl.h>

#include <utility>

namespace sdgl {
    struct App::Impl {
        Impl(backends::Backend *backend, string title, int width, int height, WindowFlags::Enum flags,
            PluginConfig plugins) : backend(backend), title(std::move(title)), width(width), height(height),
            flags(flags), plugins(std::move(plugins)), window(nullptr), args() { }
        backends::Backend *backend;
        string title;
        int width;
        int height;
        WindowFlags::Enum flags;
        PluginConfig plugins;
        Window *window;
        vector<string> args;
    };

    App::App(const string &title, const int width, const int height,
             const WindowFlags::Enum flags, const PluginConfig &plugins) :
        m(new Impl(backends::createBackend(), title, width, height, flags, plugins))
    {
    }

    App::~App()
    {
        delete m->backend;
        delete m;
    }

    int App::run(const int argc, char *argv[])
    {
        SDGL_LOG("SDGL App Init");

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

        m->window = window;
        m->args = vector<string>(argv, argv + argc);

        // Display vendor information
        SDGL_LOG("OpenGL Vendor:   {}", reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
        SDGL_LOG("OpenGL Renderer: {}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
        SDGL_LOG("GLSL Version:    {}", reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

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

        be->destroyWindow(window);
        be->shutdown();

        return ErrorCode::Ok;
    }

    void App::runOneFrame()
    {
        m->backend->processInput();
        const auto window = m->window;

        window->startFrame();
        update();
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

    Window *App::getWindow() const
    {
        return m->window;
    }

    float App::getTime() const
    {
        return m->backend->getAppTime();
    }
}

