#include "Glfw3Gamepad.h"

#include <iostream>

#include "Glfw3.h"

namespace sdgl::backends::glfw3
{
    int Glfw3Gamepad::m_btnMap[] = {
        GLFW_GAMEPAD_BUTTON_A,
        GLFW_GAMEPAD_BUTTON_B,
        GLFW_GAMEPAD_BUTTON_X,
        GLFW_GAMEPAD_BUTTON_Y,
        GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
        GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
        GLFW_GAMEPAD_BUTTON_BACK,
        GLFW_GAMEPAD_BUTTON_START,
        GLFW_GAMEPAD_BUTTON_GUIDE,
        GLFW_GAMEPAD_BUTTON_DPAD_LEFT,
        GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
        GLFW_GAMEPAD_BUTTON_DPAD_UP,
        GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
    };

    int Glfw3Gamepad::m_axisMap[] = {
        GLFW_GAMEPAD_AXIS_LEFT_X,
        GLFW_GAMEPAD_AXIS_LEFT_Y,
        GLFW_GAMEPAD_AXIS_RIGHT_X,
        GLFW_GAMEPAD_AXIS_RIGHT_Y,
        GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
        GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
    };

    bool Glfw3Gamepad::isActive() const
    {
        return glfwJoystickIsGamepad(m_id);
    }

    void Glfw3Gamepad::init(int id)
    {
        m_id = id;
    }

    void Glfw3Gamepad::processInput()
    {
#ifndef SDGL_PLATFORM_EMSCRIPTEN
        if (!isActive())
            return;

        GLFWgamepadstate state;
        if (!glfwGetGamepadState(m_id, &state))
        {
            std::cout << "Failed to get gamepad state for Gamepad " << m_id << '\n';
            return;
        }

        m_current = !m_current;
        for (int i = 0; const auto btnCode : m_btnMap)
        {
            m_btns[m_current * GamepadBtn::Count + i++] = state.buttons[btnCode];
        }

        for (int i = 0; const auto axisCode : m_axisMap)
        {
            m_axes[m_current * GamepadAxis::Count + i++] = state.axes[axisCode];
        }
#endif
    }
}
