#pragma once

#include "../../Gamepad.h"
#include <sdgl/sdglib.h>

namespace sdgl::backends::glfw3
{
    class Glfw3Gamepad {
    public:
        Glfw3Gamepad() : m_current(), m_id(-1), m_btns(GamepadBtn::Count * 2), m_axes(GamepadAxis::Count * 2) { }
        void init(int id);

        [[nodiscard]]
        int id() const { return m_id; }

        [[nodiscard]]
        bool isDown(const GamepadBtn::Enum button) const
        {
            return m_btns[m_current * GamepadBtn::Count + button];
        }

        [[nodiscard]]
        bool isUp(const GamepadBtn::Enum button) const
        {
            return !isDown(button);
        }

        [[nodiscard]]
        bool isPressed(const GamepadBtn::Enum button) const
        {
            return m_btns[m_current * GamepadBtn::Count + button] && !m_btns[!m_current * GamepadBtn::Count + button];
        }

        [[nodiscard]]
        bool isReleased(const GamepadBtn::Enum button) const
        {
            return !m_btns[m_current * GamepadBtn::Count + button] && m_btns[!m_current * GamepadBtn::Count + button];
        }

        [[nodiscard]]
        float getAxis(const GamepadAxis::Enum axis) const
        {
            return m_axes[m_current * GamepadAxis::Count + axis];
        }

        [[nodiscard]]
        float getAxisLast(const GamepadAxis::Enum axis) const
        {
            return m_axes[!m_current * GamepadAxis::Count + axis];
        }

        void processInput();

        [[nodiscard]]
        bool isActive() const;
    private:
        int m_current;
        int m_id;
        vector<bool> m_btns;
        vector<float> m_axes;
        static int m_btnMap[GamepadBtn::Count];
        static int m_axisMap[GamepadAxis::Count];
    };
}

