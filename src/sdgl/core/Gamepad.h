#pragma once
#include "GamepadConstants.h"
#include <sdgl/BitFlags.h>

namespace sdgl {
    class Gamepad {
    public:
        Gamepad();

        [[nodiscard]]
        bool isDown(GamepadBtn::Enum button) const { return m_btns.get(button); }

        [[nodiscard]]
        bool isPressed(GamepadBtn::Enum button) const { return m_btns.get(button) && !m_lastBtns.get(button); }

        [[nodiscard]]
        bool isUp(GamepadBtn::Enum button) const { return !m_btns.get(button); }

        [[nodiscard]]
        bool isReleased(GamepadBtn::Enum button) const { return !m_btns.get(button) && m_lastBtns.get(button); }

        [[nodiscard]]
        float getAxis(GamepadAxis::Enum axis) const { return m_axes[axis]; }

        [[nodiscard]]
        float getLastAxis(GamepadAxis::Enum axis) const { return m_lastAxes[axis]; }

        void preProcessInput();

        [[nodiscard]]
        bool isConnected() const { return m_controller != nullptr; }

        void close();
    private:
        friend class Backend;
        BitFlags m_btns, m_lastBtns, m_released;
        float m_axes[GamepadAxis::Count];
        float m_lastAxes[GamepadAxis::Count];

        void *m_controller; // pointer to SDL_GameController
        int m_id;

        void doButtonDown(uint8_t buttonid);
        void doButtonUp(uint8_t buttonid);
        void doAxisSet(uint8_t axisid, float value);
        void doConnect(void *controller, int instanceID);
        bool tryDisconnect(int instanceID);
        [[nodiscard]]
        auto getInstanceID() const { return m_id; }
    };
}