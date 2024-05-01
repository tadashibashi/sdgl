#pragma once
#include "sdgl/BitFlags.h"

namespace sdgl {
    struct MouseBtn {
        enum Enum {
            Left,
            Middle,
            Right,

            Count
        };
    };

    struct MouseAxis {
        enum Enum {
            ScrollX,
            ScrollY,

            Count
        };
    };

    class Mouse {
    public:
        Mouse();

        void getPosition(float *x, float *y) const;

        [[nodiscard]]
        bool isDown(MouseBtn::Enum button) const;
        [[nodiscard]]
        bool isPressed(MouseBtn::Enum button) const;
        [[nodiscard]]
        bool isUp(MouseBtn::Enum button) const;
        [[nodiscard]]
        bool isReleased(MouseBtn::Enum button) const;

        [[nodiscard]]
        bool isDoubleClicked(MouseBtn::Enum button) const;

        [[nodiscard]]
        float getAxis(MouseAxis::Enum axis) const;

        [[nodiscard]]
        float getlastAxis(MouseAxis::Enum axis) const;

        void preProcessInput();
    private:
        friend class Backend;
        void setPosition(float x, float y);

        void doButtonDown(uint8_t buttonid);
        void doButtonUp(uint8_t buttonid);
        void doButtonDoubleClick(uint8_t buttonid);

        float m_x, m_y, m_lastX, m_lastY;
        float m_scrollX, m_scrollY, m_lastScrollX, m_lastScrollY;

        BitFlags m_buttons, m_lastButtons, m_doubleClick, m_released;
    };
}
