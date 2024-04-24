#pragma once
#include "../../Mouse.h"

class GLFWwindow;

namespace sdgl::backends::glfw3 {
    class Glfw3Mouse {
    public:
        Glfw3Mouse();
        void init(GLFWwindow *window);

        [[nodiscard]]
        bool isDown(MouseBtn::Enum button) const;
        [[nodiscard]]
        bool isPressed(MouseBtn::Enum button) const;
        [[nodiscard]]
        bool isUp(MouseBtn::Enum button) const;
        [[nodiscard]]
        bool isReleased(MouseBtn::Enum button) const;

        [[nodiscard]]
        bool isHoveringWindow() const;

        [[nodiscard]]
        double x() const { return m_x; }
        [[nodiscard]]
        double y() const { return m_y; }

        [[nodiscard]]
        double scrollX() const { return m_scrollX; }
        [[nodiscard]]
        double scrollY() const { return m_scrollY; }
        [[nodiscard]]
        double scrollXLast() const { return m_lastScrollX; }
        [[nodiscard]]
        double scrollYLast() const { return m_lastScrollY; }

        void preProcessInput();
        void processInput();

    private:
        static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
        static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);

        GLFWwindow *m_window;
        double m_x, m_y, m_lastX, m_lastY;
        double m_scrollX, m_scrollY, m_lastScrollX, m_lastScrollY;
        bool m_buttons[2][(int)MouseBtn::Count];
        int m_current;
    };
}
