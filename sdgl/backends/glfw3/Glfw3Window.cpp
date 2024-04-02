#include "Glfw3Window.h"

namespace sdgl::backends::glfw3 {
    void Glfw3Window::processInput()
    {
        m_keyboard.processInput();
        m_mouse.processInput();
    }

    void Glfw3Window::preProcessInput()
    {
        m_mouse.preProcessInput();
    }

    Window &Glfw3Window::setSize(const int width, const int height)
    {
        glfwSetWindowSize(m_window, width, height);
        return *this;
    }

    void Glfw3Window::getSize(int *x, int *y) const
    {
        glfwGetWindowSize(m_window, x, y);
    }

    Window &Glfw3Window::setPosition(const int x, const int y)
    {
        glfwSetWindowPos(m_window, x, y);
        return *this;
    }

    void Glfw3Window::getPosition(int *x, int *y)
    {
        glfwGetWindowPos(m_window, x, y);
    }

    bool Glfw3Window::isCursorVisible() const
    {
        const auto cursorMode = glfwGetInputMode(m_window, GLFW_CURSOR);
        return cursorMode & GLFW_CURSOR_NORMAL;
    }

    Window &Glfw3Window::setCursorVisible(const bool value)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, value ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
        return *this;
    }

    bool Glfw3Window::shouldClose() const
    {
        return glfwWindowShouldClose(m_window);
    }

    Window & Glfw3Window::setShouldClose(const bool value)
    {
        glfwSetWindowShouldClose(m_window, value);
        return *this;
    }

    bool Glfw3Window::isHidden() const
    {
        return !glfwGetWindowAttrib(m_window, GLFW_VISIBLE);
    }

    Window &Glfw3Window::setHidden(const bool value)
    {
        glfwSetWindowAttrib(m_window, GLFW_VISIBLE, !value);
        return *this;
    }

    bool Glfw3Window::isHovered() const
    {
        return glfwGetWindowAttrib(m_window, GLFW_HOVERED);
    }

    bool Glfw3Window::isFullscreen() const
    {
        return glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED);
    }

    Window &Glfw3Window::setFullscreen(const bool value)
    {
        glfwSetWindowAttrib(m_window, GLFW_MAXIMIZED, value);
        return *this;
    }

    bool Glfw3Window::isBorderless() const
    {
        return !glfwGetWindowAttrib(m_window, GLFW_DECORATED);
    }

    Window & Glfw3Window::setBorderless(bool value)
    {
        glfwSetWindowAttrib(m_window, GLFW_DECORATED, !value);
        return *this;
    }

    bool Glfw3Window::isAlwaysOnTop() const
    {
        return glfwGetWindowAttrib(m_window, GLFW_FLOATING);
    }

    Window &Glfw3Window::setAlwaysOnTop(const bool value)
    {
        glfwSetWindowAttrib(m_window, GLFW_FLOATING, value);
        return *this;
    }

    bool Glfw3Window::isResizable() const
    {
        return glfwGetWindowAttrib(m_window, GLFW_RESIZABLE);
    }

    Window &Glfw3Window::setResizable(const bool value)
    {
        glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, value);
        return *this;
    }

    string_view Glfw3Window::getTitle() const
    {
        return m_title;
    }

    Window &Glfw3Window::setTitle(const string &title)
    {
        glfwSetWindowTitle(m_window, title.c_str());
        m_title = title;
        return *this;
    }

    bool Glfw3Window::isDown(const Key::Enum key) const
    {
        return m_keyboard.isDown(key);
    }

    bool Glfw3Window::isPressed(const Key::Enum key) const
    {
        return m_keyboard.isPressed(key);
    }

    bool Glfw3Window::isUp(const Key::Enum key) const
    {
        return m_keyboard.isUp(key);
    }

    bool Glfw3Window::isReleased(const Key::Enum key) const
    {
        return m_keyboard.isReleased(key);
    }

    bool Glfw3Window::isDown(const MouseBtn::Enum button) const
    {
        return m_mouse.isDown(button);
    }

    bool Glfw3Window::isPressed(const MouseBtn::Enum button) const
    {
        return m_mouse.isPressed(button);
    }

    bool Glfw3Window::isUp(const MouseBtn::Enum button) const
    {
        return m_mouse.isUp(button);
    }

    bool Glfw3Window::isReleased(const MouseBtn::Enum button) const
    {
        return m_mouse.isReleased(button);
    }

    float Glfw3Window::getAxis(const MouseAxis::Enum axis) const
    {
        return (axis == MouseAxis::ScrollX) ?
            static_cast<float>(m_mouse.scrollX()) :
            static_cast<float>(m_mouse.scrollY());
    }

    float Glfw3Window::getAxisLast(MouseAxis::Enum axis) const
    {
        return (axis == MouseAxis::ScrollX) ?
            static_cast<float>(m_mouse.scrollXLast()) :
            static_cast<float>(m_mouse.scrollYLast());
    }

    void Glfw3Window::getMousePosition(double *x, double *y) const
    {
        if (x)
            *x = m_mouse.x();
        if (y)
            *y = m_mouse.y();
    }

    bool Glfw3Window::isGamepadConnected(const int index) const
    {
        return m_gamepads.at(index).isActive();
    }

    bool Glfw3Window::isDown(const int index, const GamepadBtn::Enum button) const
    {
        return m_gamepads.at(index).isDown(button);
    }

    bool Glfw3Window::isPressed(const int index, const GamepadBtn::Enum button) const
    {
        return m_gamepads.at(index).isPressed(button);
    }

    float Glfw3Window::getAxis(const int index, const GamepadAxis::Enum axis) const
    {
        return m_gamepads.at(index).getAxis(axis);
    }

    float Glfw3Window::getAxisLast(const int index, const GamepadAxis::Enum axis) const
    {
        return m_gamepads.at(index).getAxisLast(axis);
    }

    void *Glfw3Window::getHandle() const
    {
        return m_window;
    }

    void Glfw3Window::swapBuffers() const
    {
#if !defined(SDGL_PLATFORM_EMSCRIPTEN)
        glfwSwapBuffers(m_window);
#endif
    }

    void Glfw3Window::makeCurrent() const
    {
        if (m_window)
            glfwMakeContextCurrent(m_window);
    }
}
