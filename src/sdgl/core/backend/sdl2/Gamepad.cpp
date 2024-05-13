#include "../../Gamepad.h"
#include <sdgl/logging.h>

#include <SDL_gamecontroller.h>


namespace sdgl {

    static constexpr uint8_t s_btnToSdlBtn[] = {
        SDL_CONTROLLER_BUTTON_A,
        SDL_CONTROLLER_BUTTON_B,
        SDL_CONTROLLER_BUTTON_X,
        SDL_CONTROLLER_BUTTON_Y,
        SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
        SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
        SDL_CONTROLLER_BUTTON_LEFTSTICK,
        SDL_CONTROLLER_BUTTON_RIGHTSTICK,
        SDL_CONTROLLER_BUTTON_BACK,
        SDL_CONTROLLER_BUTTON_START,
        SDL_CONTROLLER_BUTTON_GUIDE,
        SDL_CONTROLLER_BUTTON_DPAD_LEFT,
        SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
        SDL_CONTROLLER_BUTTON_DPAD_UP,
        SDL_CONTROLLER_BUTTON_DPAD_DOWN,
    };

    static uint8_t s_sdlBtnToBtn[SDL_CONTROLLER_BUTTON_MAX];

    static constexpr uint8_t s_axisToSdlAxis[] = {
        SDL_CONTROLLER_AXIS_LEFTX,
        SDL_CONTROLLER_AXIS_LEFTY,
        SDL_CONTROLLER_AXIS_RIGHTX,
        SDL_CONTROLLER_AXIS_RIGHTY,
        SDL_CONTROLLER_AXIS_TRIGGERLEFT,
        SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
    };

    static uint8_t s_sdlAxisToAxis[SDL_CONTROLLER_AXIS_MAX];

    Gamepad::Gamepad() :
        m_btns(GamepadBtn::Count), m_lastBtns(GamepadBtn::Count), m_released(GamepadBtn::Count),
        m_axes(), m_lastAxes(), m_controller(nullptr), m_id(-1)
    {
        if (s_sdlBtnToBtn[SDL_CONTROLLER_BUTTON_DPAD_DOWN] == 0)
        {
            for (size_t i = 0; auto btn : s_btnToSdlBtn)
            {
                s_sdlBtnToBtn[btn] = i++;
            }
        }

        if (s_sdlAxisToAxis[SDL_CONTROLLER_AXIS_TRIGGERRIGHT] == 0)
        {
            for (size_t i = 0; auto axis : s_axisToSdlAxis)
            {
                s_sdlAxisToAxis[axis] = i++;
            }
        }
    }

    void Gamepad::preProcessInput()
    {
        const auto memSize = sizeof(uint32_t) * m_btns.size();

        // update last states
        std::memcpy(m_lastBtns.data(), m_btns.data(), memSize);
        std::memcpy(m_lastAxes, m_axes, sizeof(m_axes));

        static_assert(GamepadBtn::Count <= 32 && GamepadBtn::Count > 0); // if this emits a compile error, adjust condition and handrolled code below
        {
            uint32_t *current = m_btns.data();
            uint32_t *released = m_released.data();
            auto result = ~*current | *released;
            *current = ~result;
            *released = result;
        }
    }

    void Gamepad::close()
    {
        if (m_controller)
        {
            SDL_GameControllerClose(static_cast<SDL_GameController *>(m_controller));
            m_controller = nullptr;
            m_id = -1;
        }
    }

    void Gamepad::doButtonDown(uint8_t buttonid)
    {
        const auto button = s_sdlBtnToBtn[buttonid];
        m_btns.set(button, true);
        m_released.set(button, false);
    }

    void Gamepad::doButtonUp(uint8_t buttonid)
    {
        const auto button = s_sdlBtnToBtn[buttonid];
        m_released.set(button, true);
    }

    void Gamepad::doAxisSet(uint8_t axisid, float value)
    {
        const auto axis = s_sdlAxisToAxis[axisid];
        m_axes[axis] = value;
    }

    void Gamepad::doConnect(void *controller, int instanceID)
    {
        close();
        m_controller = controller;
        m_id = instanceID;
    }
}

bool sdgl::Gamepad::tryDisconnect(int instanceID)
{
    if (m_id != instanceID) return false;

    close();
    return true;
}
