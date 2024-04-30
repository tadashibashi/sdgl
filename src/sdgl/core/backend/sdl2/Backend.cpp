#include "../Backend.h"

#include <iostream>
#include <SDL2/SDL.h>
#include <angles.h>

#include <chrono>

#include "ImGuiSdl2.h"

using namespace std::chrono_literals;
using namespace std::chrono;

namespace sdgl {
    struct Backend::Impl {
        std::vector<Window *> windows;
        time_point<high_resolution_clock> startTime;
    };

    Backend::Backend() : m(new Impl) { }
    Backend::~Backend() { delete m; }
    bool Backend::init()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER) != 0)
        {
            std::cerr << "SDL failed to initialize: " << SDL_GetError() << '\n';
            return false;
        }

        //SDL_GameControllerOpen(0)
        return true;
    }

    double Backend::getAppTime() const
    {
        return static_cast<double>(duration_cast<microseconds>(high_resolution_clock::now() - m->startTime).count()) *
            .000001;
    }

    [[nodiscard]]
    string_view Backend::name() const { return "SDL2"; }

    Window *Backend::createWindow(const string &title, int width, int height, WindowFlags::Enum flags, const PluginConfig &plugins)
    {
        uint32_t winFlags = 0;
        if (flags & WindowFlags::Resizable)
            winFlags |= SDL_WINDOW_RESIZABLE;
        if (flags & WindowFlags::Fullscreen)
            winFlags |= SDL_WINDOW_FULLSCREEN;
        if (flags & WindowFlags::Borderless)
            winFlags |= SDL_WINDOW_BORDERLESS;
        if (flags & WindowFlags::AlwaysOnTop)
            winFlags |= SDL_WINDOW_ALWAYS_ON_TOP;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 2);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

        auto win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height, winFlags | SDL_WINDOW_OPENGL);

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

        auto wrapper =  new Window(win, context);

        if (plugins.imgui)
            wrapper->plugins()->addPlugin(new ImGuiSdl2(win, context));

        if (!wrapper->plugins()->init())
        {
            // a plugin failed to init, fail fast
            destroyWindow(wrapper);
            return nullptr;
        }

        m->windows.emplace_back(wrapper);

        SDL_SetWindowData(win, "w", wrapper);

        SDL_ShowWindow(win);
        return wrapper;
    }

    void Backend::destroyWindow(Window *window)
    {
        delete window; // destructor of window handles cleanup
    }

    static Window *getWindow(uint id)
    {
        SDL_Window *sdlWindow = SDL_GetWindowFromID(id);
        if (!sdlWindow) return nullptr;

        return static_cast<Window *>(SDL_GetWindowData(sdlWindow, "w"));
    }

    void Backend::processWindowEvent(const SDL_WindowEvent &e)
    {
        auto window = getWindow(e.windowID);

        switch(e.event)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                window->setShouldClose(true);
            } break;

            case SDL_WINDOWEVENT_SIZE_CHANGED:
            {
                window->makeCurrent();
                glViewport(0, 0, e.data1, e.data2);
            } break;

            default:
                break;
        }
    }

    void Backend::processInput()
    {
        for (auto window : m->windows)
        {
            window->input()->preProcessInput();
        }

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_WINDOWEVENT:
                {
                    processWindowEvent(e.window);
                } break;

                case SDL_KEYDOWN:
                {
                    auto window = getWindow(e.key.windowID);
                    auto keyboard = window->input()->keyboard();
                    if (!keyboard)
                        break;
                    keyboard->doKeyDown(e.key.keysym.scancode);
                } break;

                case SDL_KEYUP:
                {
                    auto window = getWindow(e.key.windowID);
                    auto keyboard = window->input()->keyboard();
                    if (!keyboard)
                        break;
                    keyboard->doKeyUp(e.key.keysym.scancode);
                }
            }
        }
    }

    void Backend::shutdown()
    {
        for (auto window : m->windows)
        {
            destroyWindow(window);
        }
        m->windows.clear();

        SDL_Quit();
    }
}
