#include "Glfw3Mouse.h"
#include "Glfw3.h"
#include "Glfw3Backend.h"
#include "Glfw3Window.h"

namespace sdgl::backends::glfw3 {

    static int s_mouseButtonMap[] = {
        GLFW_MOUSE_BUTTON_LEFT,
        GLFW_MOUSE_BUTTON_RIGHT,
        GLFW_MOUSE_BUTTON_MIDDLE
    };

    Glfw3Mouse::Glfw3Mouse() : m_window{}, m_x{}, m_y{}, m_scrollX{}, m_scrollY{}, m_buttons{}, m_current{},
        m_lastX{}, m_lastY{}, m_lastScrollX{}, m_lastScrollY{}
    {
    }

    void Glfw3Mouse::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
        const auto glfwWindow = static_cast<Glfw3Window *>(glfwGetWindowUserPointer(window));
        auto &mouse = glfwWindow->m_mouse;
        mouse.m_scrollX += xoffset;
        mouse.m_scrollY += yoffset;
    }

    void Glfw3Mouse::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
    {
        const auto glfwWindow = static_cast<Glfw3Window *>(glfwGetWindowUserPointer(window));
        auto &mouse = glfwWindow->m_mouse;
        mouse.m_x = xpos;
        mouse.m_y = ypos;
    }


    void Glfw3Mouse::init(GLFWwindow *window)
    {
        glfwSetCursorPosCallback(window, cursorPositionCallback);
        glfwSetScrollCallback(window, scrollCallback);
        glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, true);

        m_window = window;
    }

    bool Glfw3Mouse::isHoveringWindow() const
    {
        return glfwGetWindowAttrib(m_window, GLFW_HOVERED);
    }

    void Glfw3Mouse::preProcessInput()
    {
        m_lastX = m_x;
        m_lastY = m_y;
        m_lastScrollX = m_scrollX;
        m_lastScrollY = m_scrollY;
        m_scrollX = 0;
        m_scrollY = 0;
    }

    void Glfw3Mouse::processInput()
    {
        m_current = !m_current;
        for (int i = 0; const auto button : s_mouseButtonMap)
        {
            m_buttons[m_current][i++] = glfwGetMouseButton(m_window, button);
        }
    }

    bool Glfw3Mouse::isDown(const MouseBtn::Enum button) const
    {
        return m_buttons[m_current][button];
    }

    bool Glfw3Mouse::isPressed(const MouseBtn::Enum button) const
    {
        return m_buttons[m_current][button] && !m_buttons[!m_current][button];
    }

    bool Glfw3Mouse::isUp(const MouseBtn::Enum button) const
    {
        return !m_buttons[m_current][button];
    }

    bool Glfw3Mouse::isReleased(const MouseBtn::Enum button) const
    {
        return !m_buttons[m_current][button] && m_buttons[!m_current][button];
    }
}

