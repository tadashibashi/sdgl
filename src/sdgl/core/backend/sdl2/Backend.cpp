#include "../Backend.h"

#include <iostream>
#include <sdgl/logging.h>
#include <sdgl/angles.h>

#include <filesystem>
#include <chrono>
#include <SDL.h>

#include "ImGuiSdl2.h"

using namespace std::chrono_literals;
using namespace std::chrono;

static constexpr int MaxGamepads = 4;

namespace sdgl {
    struct Backend::Impl {
        Impl() : windows(), gamepads{}, startTime() { }

        vector<Window *> windows;
        Gamepad gamepads[MaxGamepads];
        time_point<high_resolution_clock> startTime;
    };

    Backend::Backend() : m(new Impl) { }
    Backend::~Backend() { delete m; }

    bool Backend::init()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER) != 0)
        {
            SDGL_ERROR("SDL failed to initialize: {}", SDL_GetError());
            return false;
        }

        // Load game controller mappings
        if (std::filesystem::exists("gamecontrollerdb.txt"))
        {
            SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
        }

        // Load open game controllers
        for (int i = 0; i < MaxGamepads; ++i)
        {
            auto controller = SDL_GameControllerOpen(i);
            if (controller)
            {
                m->gamepads[i].doConnect(controller);
            }
        }

        m->startTime = high_resolution_clock::now();
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
            SDGL_ERROR("SDL window failed to create: {}", SDL_GetError());
            return nullptr;
        }

        auto context = SDL_GL_CreateContext(win);
        if (!context)
        {
            SDGL_ERROR("SDL failed to create gl context: {}", SDL_GetError());
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
        glEnable(GL_BLEND); GL_ERR_CHECK();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); GL_ERR_CHECK();

        glEnable(GL_DEPTH_TEST); GL_ERR_CHECK();


        // Viewport
        int drawW, drawH;
        SDL_GL_GetDrawableSize(win, &drawW, &drawH);
        glViewport(0, 0, drawW, drawH); GL_ERR_CHECK();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_ERR_CHECK();

        auto wrapper =  new Window(win, context, m->gamepads);

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

            case SDL_WINDOWEVENT_RESIZED:
            {
                window->makeCurrent();
                glViewport(0, 0, e.data1, e.data2); GL_ERR_CHECK();
            } break;

            default:
                break;
        }
    }

    // normalize s16 to float ranging from -1 to 1
    static float normalizeS16(int16_t value)
    {
        return value < 0 ? (float)value / (float)-INT16_MIN :
            (float)value / (float)INT16_MAX;
    }

    void Backend::processInput()
    {
        for (auto window : m->windows)
        {
            auto input = const_cast<InputManager *>(window->input());
            if (input)
                input->preProcessInput();
        }

        for (int i = 0; auto &gamepad : m->gamepads)
        {
            if (gamepad.isConnected())
                gamepad.preProcessInput();
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

                    if (e.button.clicks == 2)
                        mouse->doButtonDoubleClick(e.button.button);
                    else
                        mouse->doButtonDown(e.button.button);
                } break;

                case SDL_MOUSEBUTTONUP:
                {
                    // mouse ups should affect all windows to prevent stuck state if mouse leaves window
                    for (auto window : m->windows)
                    {
                        auto mouse = const_cast<Mouse *>(window->input()->mouse());
                        if (!mouse)
                            break;
                        mouse->doButtonUp(e.button.button);
                    }

                } break;

                case SDL_CONTROLLERBUTTONDOWN:
                {
                    const auto id = e.cbutton.which;
                    if (id == -1 || id > 3) break;

                    auto &gamepad = m->gamepads[id];
                    gamepad.doButtonDown(e.cbutton.button);
                } break;

                case SDL_CONTROLLERBUTTONUP:
                {
                    const auto id = e.cbutton.which;
                    if (id == -1 || id > 3) break;

                    auto &gamepad = m->gamepads[id];
                    gamepad.doButtonUp(e.cbutton.button);
                } break;

                case SDL_CONTROLLERDEVICEREMOVED:
                {
                    const auto id = e.cdevice.which;
                    if (id == -1 || id > 3) break;

                    auto &gamepad = m->gamepads[id];
                    gamepad.close();

                } break;

                case SDL_CONTROLLERDEVICEADDED:
                {
                    const auto id = e.cdevice.which;
                    if (id < 0 || id >= MaxGamepads) break; // only allow supported range of controllers

                    // retrieve connected controller
                    auto controller = SDL_GameControllerOpen(id);
                    if (controller)
                        m->gamepads[id].doConnect(controller);

                } break;

                case SDL_CONTROLLERAXISMOTION:
                {
                    const auto id = e.caxis.which;
                    if (id < 0 || id >= MaxGamepads) break; // only allow supported range of controllers

                    m->gamepads[id].doAxisSet(e.caxis.axis, normalizeS16(e.caxis.value));
                } break;

                default:
                    break;
            }
        }
    }

    void Backend::shutdown()
    {
        for (auto gamepad : m->gamepads)
        {
            gamepad.close();
        }

        for (auto window : m->windows)
        {
            delete window;
        }
        m->windows.clear();
        SDL_Quit();
    }
}
