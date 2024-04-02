#include "Sdl3Backend.h"

#include <iostream>
#include <sdgl/gl.h>
#include <SDL3/SDL.h>

#include "ImGuiSdl3.h"
#include "Sdl3Window.h"

namespace sdgl::backends {
    Backend *createBackend() { return new sdl3::Sdl3Backend; }
}

namespace sdgl::backends::sdl3 {
    Sdl3Backend::Sdl3Backend()
    {
    }

    Sdl3Backend::~Sdl3Backend()
    {
    }

    bool Sdl3Backend::init()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD) != 0)
        {
            std::cerr << "SDL3 failed to init: error: " << SDL_GetError() << '\n';
            return false;
        }

        for (int i = 0; i < 4; ++i)
        {
            auto gp = SDL_OpenGamepad(i);
            m_gamepads.emplace_back(gp);
        }


        return true;
    }

    double Sdl3Backend::getAppTime() const
    {
        return SDL_GetTicksNS() * 0.000000001L;
    }

    Window * Sdl3Backend::createWindow(const string &title, int width, int height, WindowFlags::Enum flags, const PluginConfig &plugins)
    {
        uint32_t winFlags = 0;
        if (flags & WindowFlags::Resizable)
            winFlags |= SDL_WINDOW_RESIZABLE;
        if (flags & WindowFlags::Fullscreen)
            winFlags |= SDL_WINDOW_FULLSCREEN;
        if (flags & WindowFlags::Borderless)
            winFlags |= SDL_WINDOW_BORDERLESS;
        if (flags & WindowFlags::Transparent)
            winFlags |= SDL_WINDOW_TRANSPARENT;
        if (flags & WindowFlags::AlwaysOnTop)
            winFlags |= SDL_WINDOW_ALWAYS_ON_TOP;

// #ifdef __APPLE__
//         SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
// #else
//         SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
// #endif
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_EGL_PLATFORM, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 2);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

        auto win = SDL_CreateWindow(title.c_str(), width, height, winFlags | SDL_WINDOW_OPENGL);
        if (win == nullptr)
        {
            std::cerr << "SDL3 window failed to create: " << SDL_GetError() << '\n';
            return nullptr;
        }

        auto context = SDL_GL_CreateContext(win);
        if (!context)
        {
            std::cerr << "SDL3 failed to create gl context: " << SDL_GetError() << '\n';
            SDL_DestroyWindow(win);
            return nullptr;
        }
        SDL_GL_MakeCurrent(win, context);

        // Vsync
        if (SDL_GL_SetSwapInterval(-1) < 0)
        {
            SDL_GL_SetSwapInterval(1);
        }

        // Blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Viewport
        SDL_GetWindowSize(win, &width, &height);
        glViewport(0, 0, width, height);

        auto wrapper =  new Sdl3Window(win, context);

        if (plugins.imgui)
            wrapper->plugins()->addPlugin(new ImGuiSdl3(win, context));

        if (!wrapper->plugins()->init())
        {
            // a plugin failed to init, fail fast
            destroyWindow(wrapper);
            return nullptr;
        }

        m_windows.emplace_back(wrapper);

        SDL_ShowWindow(win);
        return wrapper;
    }

    void Sdl3Backend::destroyWindow(Window *window)
    {
        auto sdlWindow = dynamic_cast<Sdl3Window *>(window);
        if (!sdlWindow)
        {
            std::cerr << "Wrong type passed to Sdl3Backend::destroyWindow!\n";
            return;
        }

        {
            auto it = std::find(m_windows.begin(), m_windows.end(), window);
            if (it != m_windows.end())
                m_windows.erase(it);
        }

        window->plugins()->shutdown();

        SDL_GL_DeleteContext(sdlWindow->m_context);
        SDL_DestroyWindow(sdlWindow->m_window);
        delete window;
    }

    void Sdl3Backend::processInput()
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            for (auto window : m_windows)
                window->plugins()->processEvent(&ev);

            switch(ev.type)
            {
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                {
                    auto window = SDL_GetWindowFromID(ev.window.windowID);
                    auto propsID = SDL_GetWindowProperties(window);
                    auto wrapper = static_cast<Sdl3Window *>(SDL_GetProperty(propsID, "wrapper", nullptr));
                    if (wrapper)
                    {
                        wrapper->setShouldClose(true);
                    }
                }
                    break;
            }
        }
    }

    void Sdl3Backend::shutdown()
    {
        for (const auto gp : m_gamepads)
        {
            SDL_CloseGamepad(gp);
        }

        SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD);
        SDL_Quit();
    }
}

