#include "InputManager.h"

sdgl::InputManager::InputManager() : m_keyboard(), m_mouse(), m_gamepads()
{

}

void sdgl::InputManager::init(InputInit::Flags initFlags, const Gamepad *gamepads)
{
    if (initFlags & InputInit::Keyboard)
        m_keyboard = Keyboard();

    if (initFlags & InputInit::Mouse)
        m_mouse = Mouse();

    if (initFlags & InputInit::Gamepad)
    {
        m_gamepads = gamepads;
    }
}

void sdgl::InputManager::preProcessInput()
{
    if (m_keyboard)
        m_keyboard->preProcessInput();

    if (m_mouse)
        m_mouse->preProcessInput();
}
