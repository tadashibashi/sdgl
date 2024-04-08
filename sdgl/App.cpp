#include "App.h"

#include <sdgl/backends/Backend.h>
#include <sdgl/logging.h>
#include <sdgl/platform.h>

#include <sdgl/gl.h>

namespace sdgl {
    struct App::Impl {
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
             const WindowFlags::Enum flags, const PluginConfig &plugins) : m(new Impl)
    {
        m->title = title;
        m->width = width;
        m->height = height;
        m->flags = flags;
        m->plugins = plugins;
        m->backend = backends::createBackend();
        m->window = nullptr;
    }

    App::~App()
    {
        delete m->backend;
        delete m;
    }

    int App::run(int argc, char *argv[])
    {
        const auto be = m->backend;
        if (!be->init())
            return ErrorCode::BackendInitError;

        const auto window = be->createWindow(m->title, m->width, m->height, m->flags, m->plugins);
        if (!window)
        {
            be->shutdown();
            return ErrorCode::CreateWindowFailed;
        }

        m->window = window;
        m->args = vector<string>(argv, argv + argc);

        for (int i = 0; auto &arg : m->args)
            SDGL_LOG("ARG {}: {}", i++, arg);

        // debug display vendor information
        SDGL_LOG("OpenGL Vendor: {}", reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
        SDGL_LOG("OpenGL Renderer: {}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
        SDGL_LOG("GLSL Version: {}", reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

        if (!init())
        {
            be->destroyWindow(window);
            be->shutdown();
            return ErrorCode::AppInitError;
        }

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

