#include "Sdl3Window.h"

#include <../../../graphics/gl.h>
#include <SDL3/SDL.h>

namespace sdgl::backends::sdl3 {
    Sdl3Window::Sdl3Window(SDL_Window *window, SDL_GLContext context)
        : m_window(window), m_context(context), m_shouldClose()
    {
        auto props = SDL_GetWindowProperties(window);
        SDL_SetProperty(props, "wrapper", this);
    }

    Window &Sdl3Window::setSize(const int width, const int height)
    {
        SDL_SetWindowSize(m_window, width, height);
        return *this;
    }

    void Sdl3Window::getSize(int *width, int *height) const
    {
        SDL_GetWindowSize(m_window, width, height);
    }

    Window &Sdl3Window::setPosition(const int x, const int y)
    {
        SDL_SetWindowPosition(m_window, x, y);
        return *this;
    }

    void Sdl3Window::getPosition(int *x, int *y)
    {
        SDL_GetWindowPosition(m_window, x, y);
    }

    bool Sdl3Window::isCursorVisible() const
    {
        return SDL_CursorVisible();
    }

    Window &Sdl3Window::setCursorVisible(const bool value)
    {
        if (value)
            SDL_ShowCursor();
        else
            SDL_HideCursor();
        return *this;
    }

    bool Sdl3Window::shouldClose() const
    {
        return m_shouldClose;
    }

    Window & Sdl3Window::setShouldClose(const bool value)
    {
        m_shouldClose = value;
        return *this;
    }

    bool Sdl3Window::isHidden() const
    {
        return SDL_GetWindowFlags(m_window) & SDL_WINDOW_HIDDEN;
    }

    Window &Sdl3Window::setHidden(bool value)
    {
        if (value)
            SDL_ShowWindow(m_window);
        else
            SDL_HideWindow(m_window);

        return *this;
    }

    bool Sdl3Window::isHovered() const
    {
        // Assumes events are being polled

        float mouseX, mouseY;
        SDL_GetGlobalMouseState(&mouseX, &mouseY);

        int x, y, w, h;
        SDL_GetWindowPosition(m_window, &x, &y);
        SDL_GetWindowSize(m_window, &w, &h);

        return !(mouseX < x || mouseX > x + w || mouseY < y || mouseY > y + h);
    }

    bool Sdl3Window::isFullscreen() const
    {
        return SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN;
    }

    Window &Sdl3Window::setFullscreen(bool value)
    {
        SDL_SetWindowFullscreen(m_window, value);
        return *this;
    }

    bool Sdl3Window::isBorderless() const
    {
        return SDL_GetWindowFlags(m_window) & SDL_WINDOW_BORDERLESS;
    }

    Window & Sdl3Window::setBorderless(const bool value)
    {
        SDL_SetWindowBordered(m_window, !value);
        return *this;
    }

    bool Sdl3Window::isAlwaysOnTop() const
    {
        return SDL_GetWindowFlags(m_window) & SDL_WINDOW_ALWAYS_ON_TOP;
    }

    Window & Sdl3Window::setAlwaysOnTop(const bool value)
    {
        SDL_SetWindowAlwaysOnTop(m_window, value);
        return *this;
    }

    bool Sdl3Window::isResizable() const
    {
        return SDL_GetWindowFlags(m_window) & SDL_WINDOW_RESIZABLE;
    }

    Window &Sdl3Window::setResizable(const bool value)
    {
        SDL_SetWindowResizable(m_window, value);
        return *this;
    }

    string_view Sdl3Window::getTitle() const
    {
        return SDL_GetWindowTitle(m_window);
    }

    Window & Sdl3Window::setTitle(const string &title)
    {
        SDL_SetWindowTitle(m_window, title.c_str());
        return *this;
    }

    bool Sdl3Window::isDown(Key::Enum key) const
    {
        return false;
    }

    bool Sdl3Window::isPressed(Key::Enum key) const
    {
        return false;
    }

    bool Sdl3Window::isUp(Key::Enum key) const
    {
        return false;
    }

    bool Sdl3Window::isReleased(Key::Enum key) const
    {
        return false;
    }

    bool Sdl3Window::isDown(MouseBtn::Enum button) const
    {
        return false;
    }

    bool Sdl3Window::isPressed(MouseBtn::Enum button) const
    {
        return false;
    }

    bool Sdl3Window::isUp(MouseBtn::Enum button) const
    {
        return false;
    }

    bool Sdl3Window::isReleased(MouseBtn::Enum button) const
    {
        return false;
    }

    float Sdl3Window::getAxis(MouseAxis::Enum axis) const
    {
        return 0;
    }

    void Sdl3Window::getMousePosition(double *x, double *y) const
    {

    }

    void * Sdl3Window::getHandle() const
    {
        return m_window;
    }

    void Sdl3Window::swapBuffers() const
    {
        SDL_GL_SwapWindow(m_window);
    }

    void Sdl3Window::makeCurrent() const
    {
        SDL_GL_MakeCurrent(m_window, m_context);
    }

    float Sdl3Window::getAxisLast(MouseAxis::Enum axis) const
    {
        return 0;
    }

    bool Sdl3Window::isGamepadConnected(int index) const
    {
        return false;
    }

    bool Sdl3Window::isDown(int index, GamepadBtn::Enum button) const
    {
        return false;
    }

    bool Sdl3Window::isPressed(int index, GamepadBtn::Enum button) const
    {
        return false;
    }

    float Sdl3Window::getAxis(int index, GamepadAxis::Enum axis) const
    {
        return 0;
    }

    float Sdl3Window::getAxisLast(int index, GamepadAxis::Enum axis) const
    {
        return 0;
    }
}
