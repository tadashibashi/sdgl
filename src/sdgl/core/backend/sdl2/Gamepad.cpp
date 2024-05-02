#include "../../Gamepad.h"
#include <SDL2/SDL_gamecontroller.h>

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

sdgl::Gamepad::Gamepad() :
    m_btns(GamepadBtn::Count), m_lastBtns(GamepadBtn::Count), m_released(GamepadBtn::Count),
    m_axes(), m_lastAxes(), m_connected(false), m_controller()
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

void sdgl::Gamepad::preProcessInput()
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

void sdgl::Gamepad::close()
{
    SDL_GameControllerClose(static_cast<SDL_GameController *>(m_controller));
}

void sdgl::Gamepad::doButtonDown(uint8_t buttonid)
{
    const auto button = s_sdlBtnToBtn[buttonid];
    m_btns.set(button, true);
    m_released.set(button, false);
}

void sdgl::Gamepad::doButtonUp(uint8_t buttonid)
{
    const auto button = s_sdlBtnToBtn[buttonid];
    m_released.set(button, true);
}

void sdgl::Gamepad::doAxisSet(uint8_t axisid, float value)
{
    const auto axis = s_sdlAxisToAxis[axisid];
    m_axes[axis] = value;
}

void sdgl::Gamepad::doConnect(bool isConnected)
{
    m_connected = isConnected;
}
