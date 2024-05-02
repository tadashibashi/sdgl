#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "GamepadConstants.h"

#include <optional>

#include "Gamepad.h"

namespace sdgl {
    struct InputInit {
        enum Flags : uint {
            None = 0,
            Keyboard = 1,
            Mouse = 1 << 1,
            Gamepad = 1 << 2,
            All = Keyboard | Mouse | Gamepad,
        };
    };

    inline constexpr InputInit::Flags operator |(InputInit::Flags a, InputInit::Flags b)
    {
        return static_cast<InputInit::Flags>(static_cast<uint>(a) | static_cast<uint>(b));
    }

    inline constexpr InputInit::Flags &operator |=(InputInit::Flags &a, InputInit::Flags b)
    {
        return a = a | b;
    }

    class InputManager {
    public:
        InputManager();

        void init(InputInit::Flags initFlags, const Gamepad *gamepads);
        void preProcessInput();

        // ===== Window Input =====

        [[nodiscard]]
        bool isDown(Key::Enum key) const { return m_keyboard->isDown(key); }

        [[nodiscard]]
        bool isPressed(Key::Enum key) const { return m_keyboard->isPressed(key); }

        [[nodiscard]]
        bool isUp(Key::Enum key) const { return m_keyboard->isUp(key); }

        [[nodiscard]]
        bool isReleased(Key::Enum key) const { return m_keyboard->isReleased(key); }

        [[nodiscard]]
        bool isDown(MouseBtn::Enum button) const { return m_mouse->isDown(button); }

        [[nodiscard]]
        bool isPressed(MouseBtn::Enum button) const { return m_mouse->isPressed(button); }

        [[nodiscard]]
        bool isUp(MouseBtn::Enum button) const { return m_mouse->isUp(button); }

        [[nodiscard]]
        bool isReleased(MouseBtn::Enum button) const { return m_mouse->isReleased(button); }

        [[nodiscard]]
        bool isDoubleClicked(MouseBtn::Enum button) const { return m_mouse->isDoubleClicked(button); }

        [[nodiscard]]
        float getAxis(MouseAxis::Enum axis) const { return m_mouse->getAxis(axis); }

        [[nodiscard]]
        float getLastAxis(MouseAxis::Enum axis) const { return m_mouse->getlastAxis(axis); }

        void getMousePosition(float *x, float *y) const { return m_mouse->getPosition(x, y); }

        [[nodiscard]]
        bool isGamepadConnected(int index) const { return m_gamepads[index].isConnected(); };
        [[nodiscard]]
        bool isDown(int index, GamepadBtn::Enum button) const { return m_gamepads[index].isDown(button); }
        [[nodiscard]]
        bool isPressed(int index, GamepadBtn::Enum button) const { return m_gamepads[index].isPressed(button); }
        [[nodiscard]]
        float getAxis(int index, GamepadAxis::Enum axis) const { return m_gamepads[index].getAxis(axis); }
        [[nodiscard]]
        float getAxisLast(int index, GamepadAxis::Enum axis) const { return m_gamepads[index].getLastAxis(axis); }

        [[nodiscard]]
        const Keyboard *keyboard() const { return &m_keyboard.value(); }
        [[nodiscard]]
        const Mouse *mouse() const { return &m_mouse.value(); }
        [[nodiscard]]
        const Gamepad *gamepad(int index) const { return m_gamepads + index; }

        [[nodiscard]]
        bool gamepadsActive() const { return static_cast<bool>(m_gamepads); }
    private:
        std::optional<Keyboard> m_keyboard;
        std::optional<Mouse> m_mouse;
        const Gamepad *m_gamepads;
    };
}
