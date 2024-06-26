#include "../../Window.h"
#include <SDL_video.h>

#include "../../InputManager.h"

#include <sdgl/graphics/Color.h>

#include <angles.h>
#include <SDL_events.h>
#include <SDL_mouse.h>

namespace sdgl {
    struct Window::Impl {
        Impl(SDL_Window *window, SDL_GLContext context, const Gamepad *gamepads) : window(window), context(context), shouldClose(), input() {
            input.init(InputInit::All, gamepads);
        }

        SDL_Window *window;
        SDL_GLContext context;
        InputManager input;
        bool shouldClose;
    };

    Window::Window(SDL_Window *window, SDL_GLContext context, const Gamepad *gamepads) : m(new Impl(window, context, gamepads)) { }
    Window::~Window()
    {
        m_plugins.shutdown();
        SDL_GL_DeleteContext(m->context);
        SDL_DestroyWindow(m->window);
        delete m;
    }

    Window &Window::setSize(int x, int y)
    {
        SDL_SetWindowSize(m->window, x, y);
        return *this;
    }

    void Window::getSize(int *x, int *y) const
    {
        SDL_GetWindowSize(m->window, x, y);
    }

    void Window::getFrameBufferSize(int *x, int *y) const
    {
        SDL_GL_GetDrawableSize(m->window, x, y);
    }

    Window &Window::setPosition(int x, int y)
    {
        SDL_SetWindowPosition(m->window, x, y);
        return *this;
    }

    void Window::getPosition(int *x, int *y) const
    {
        SDL_GetWindowPosition(m->window, x, y);
    }

    bool Window::shouldClose() const
    {
        return m->shouldClose;
    }

    Window &Window::setShouldClose(bool value)
    {
        m->shouldClose = value;
        return *this;
    }

    bool Window::isHidden() const
    {
        auto flags = SDL_GetWindowFlags(m->window);
        return flags & SDL_WINDOW_HIDDEN;
    }

    Window &Window::setHidden(bool value)
    {
        if (value)
            SDL_HideWindow(m->window);
        else
            SDL_ShowWindow(m->window);
        return *this;
    }

    bool Window::isFullscreen() const
    {
        return SDL_GetWindowFlags(m->window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    Window &Window::setFullscreen(bool value)
    {
        SDL_SetWindowFullscreen(m->window, value ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
        return *this;
    }

    bool Window::isBorderless() const
    {
        return SDL_GetWindowFlags(m->window) & SDL_WINDOW_BORDERLESS;
    }

    Window &Window::setBorderless(bool value)
    {
        SDL_SetWindowBordered(m->window, static_cast<SDL_bool>(!value));
        return *this;
    }

    bool Window::isAlwaysOnTop() const
    {
        return SDL_GetWindowFlags(m->window) & SDL_WINDOW_ALWAYS_ON_TOP;
    }

    Window &Window::setAlwaysOnTop(bool value)
    {
        SDL_SetWindowAlwaysOnTop(m->window, static_cast<SDL_bool>(value));
        return *this;
    }

    bool Window::isResizable() const
    {
        return SDL_GetWindowFlags(m->window) & SDL_WINDOW_RESIZABLE;
    }

    Window &Window::setResizable(bool value)
    {
        SDL_SetWindowResizable(m->window, static_cast<SDL_bool>(value));
        return *this;
    }

    string_view Window::getTitle() const
    {
        return SDL_GetWindowTitle(m->window);
    }

    Window &Window::setTitle(const string &title)
    {
        SDL_SetWindowTitle(m->window, title.c_str());
        return *this;
    }

    bool Window::getMouseGrabMode() const
    {
        return static_cast<bool>(SDL_GetWindowMouseGrab(m->window));
    }

    Window & Window::setMouseGrabMode(bool value)
    {
        SDL_SetWindowMouseGrab(m->window, static_cast<SDL_bool>(value));
        return *this;
    }

    bool Window::isMouseVisible() const
    {
        return static_cast<bool>(SDL_ShowCursor(SDL_QUERY));
    }

    Window & Window::setMouseVisible(bool value)
    {
        SDL_ShowCursor(value ? SDL_ENABLE : SDL_DISABLE);
        return *this;
    }

    void *Window::getHandle() const
    {
        return m->window;
    }

    void Window::swapBuffers() const
    {
        SDL_GL_SwapWindow(m->window);
    }

    void Window::makeCurrent() const
    {
        SDL_GL_MakeCurrent(m->window, m->context);
    }

    void Window::clear(const Color &color)
    {
        makeCurrent();
        glClearColor(
            static_cast<float>(color.r) / 255.f,
            static_cast<float>(color.g) / 255.f,
            static_cast<float>(color.b) / 255.f,
            static_cast<float>(color.a) / 255.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    const InputManager *Window::input() const
    {
        return &m->input;
    }
}

