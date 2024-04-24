#pragma once

#include "../../Key.h"

class GLFWwindow;

namespace sdgl::backends::glfw3
{
    class Glfw3Keyboard {
    public:
        Glfw3Keyboard() : m_window(), m_current(), m_keys(Key::Count * 2) { }
        void init(GLFWwindow *window);

        [[nodiscard]]
        bool isDown(const Key::Enum key) const
        {
            return m_keys[m_current * Key::Count + key];
        }

        [[nodiscard]]
        bool isUp(const Key::Enum key) const
        {
            return !isDown(key);
        }

        [[nodiscard]]
        bool isPressed(const Key::Enum key) const
        {
            return m_keys[m_current * Key::Count + key] && !m_keys[!m_current * Key::Count + key];
        }

        [[nodiscard]]
        bool isReleased(const Key::Enum key) const
        {
            return !m_keys[m_current * Key::Count + key] && m_keys[!m_current * Key::Count + key];
        }

        void processInput();
    private:
        GLFWwindow *m_window;
        int m_current;
        vector<bool> m_keys;
        static int s_keyMap[Key::Count];
    };
}
