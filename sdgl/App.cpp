#include "App.h"

#include "logging.h"
#include "backends/Backend.h"
#include "gl.h"

#if defined(SDGL_PLATFORM_EMSCRIPTEN)
#include <emscripten/emscripten.h>
#endif

namespace sdgl {
    struct App::Impl {
        backends::Backend *backend;
        string title;
        int width;
        int height;
        WindowFlags::Enum flags;
        PluginConfig plugins;
        Window *window;
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

#ifdef SDGL_PLATFORM_EMSCRIPTEN
    void App::emMainLoop(void *arg)
    {
        const auto app = static_cast<App *>(arg);
        app->runOneFrame();
    }
#endif
    void App::run()
    {
        const auto be = m->backend;
        if (!be->init())
            return;

        const auto window = be->createWindow(m->title, m->width, m->height, m->flags, m->plugins);
        if (!window)
        {
            be->shutdown();
            return;
        }

        m->window = window;

        // debug display vendor information
        SDGL_LOG("OpenGL Vendor: {}", reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
        SDGL_LOG("OpenGL Renderer: {}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
        SDGL_LOG("GLSL Version: {}", reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

        init();
#ifdef SDGL_PLATFORM_EMSCRIPTEN
        emscripten_set_main_loop_arg(emMainLoop, this, -1, 1);
#else
        while (!window->shouldClose())
        {
            runOneFrame();
        }
#endif
        shutdown();

        be->destroyWindow(window);
        be->shutdown();
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
}

