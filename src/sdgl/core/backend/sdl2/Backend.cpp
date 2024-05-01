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

        m->startTime = high_resolution_clock::now();

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

    Window *Backend::createWindow(const string &title, int width, int height, WindowInit::Flags flags, const PluginConfig &plugins)
    {
        uint32_t winFlags = 0;
        if (flags & WindowInit::Resizable)
            winFlags |= SDL_WINDOW_RESIZABLE;
        if (flags & WindowInit::Fullscreen)
            winFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        if (flags & WindowInit::Borderless)
            winFlags |= SDL_WINDOW_BORDERLESS;
        if (flags & WindowInit::AlwaysOnTop)
            winFlags |= SDL_WINDOW_ALWAYS_ON_TOP;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 2);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        auto win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height, winFlags | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);

        if (win == nullptr)
        {
            std::cerr << "SDL window failed to create: " << SDL_GetError() << '\n';
            return nullptr;
        }

        auto context = SDL_GL_CreateContext(win);
        if (!context)
        {
            std::cerr << "SDL failed to create gl context: " << SDL_GetError() << '\n';
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

        glEnable(GL_DEPTH_TEST);


        // Viewport
        SDL_GL_GetDrawableSize(win, &width, &height);
        glViewport(0, 0, width, height);


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        auto it = std::find_if(m->windows.begin(), m->windows.end(),
            [window](Window *curWindow) { return curWindow == window; });
        if (it != m->windows.end())
            m->windows.erase(it);
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
        if (!window)
            return;

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
            const_cast<InputManager *>(window->input())->preProcessInput();
        }

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            for (auto window : m->windows)
                window->plugins()->processEvent(&e);

            switch(e.type)
            {
                case SDL_QUIT:
                {
                    for (auto window : m->windows)
                        window->setShouldClose(true);
                } break;

                case SDL_WINDOWEVENT:
                {
                    processWindowEvent(e.window);
                } break;

                case SDL_KEYDOWN:
                {
                    auto window = getWindow(e.key.windowID);
                    if (!window)
                        break;
                    auto keyboard = const_cast<Keyboard *>(window->input()->keyboard());
                    if (!keyboard || e.key.repeat)
                        break;
                    keyboard->doKeyDown(e.key.keysym.scancode);
                } break;

                case SDL_KEYUP:
                {
                    // key ups should affect all windows to prevent stuck state if mouse leaves window
                    for (auto window : m->windows)
                    {
                        auto keyboard = const_cast<Keyboard *>(window->input()->keyboard());
                        if (!keyboard || e.key.repeat) // TODO: remove this? I don't think keyups can be repeated
                            break;
                        keyboard->doKeyUp(e.key.keysym.scancode);
                    }
                } break;

                case SDL_MOUSEMOTION:
                {
                    auto window = getWindow(e.button.windowID);
                    if (!window)
                        break;
                    auto mouse = const_cast<Mouse *>(window->input()->mouse());
                    if (!mouse)
                        break;
                    mouse->setPosition(static_cast<float>(e.button.x), static_cast<float>(e.button.y));
                } break;

                case SDL_MOUSEBUTTONDOWN:
                {
                    auto window = getWindow(e.button.windowID);
                    if (!window)
                        break;
                    auto mouse = const_cast<Mouse *>(window->input()->mouse());
                    if (!mouse)
                        break;
                    SDGL_LOG("Mouse button down: {}", e.button.button);
                    if (e.button.clicks == 2)
                        mouse->doButtonDoubleClick(e.button.button);
                    else
                        mouse->doButtonDown(e.button.button);
                } break;

                case SDL_MOUSEBUTTONUP:
                {
                    SDGL_LOG("Mouse button UP: {}", e.button.button);
                    // mouse ups should affect all windows to prevent stuck state if mouse leaves window
                    for (auto window : m->windows)
                    {
                        auto mouse = const_cast<Mouse *>(window->input()->mouse());
                        if (!mouse)
                            break;
                        mouse->doButtonUp(e.button.button);
                    }

                } break;

                default:
                    break;
            }
        }
    }

    void Backend::shutdown()
    {
        for (auto window : m->windows)
        {
            delete window;
        }
        m->windows.clear();
        SDL_Quit();
    }
}
