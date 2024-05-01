#include "InputManager.h"

sdgl::InputManager::InputManager() : m_keyboard(), m_mouse()
{

}

void sdgl::InputManager::init(InputInit::Flags initFlags)
{
    if (initFlags & InputInit::Keyboard)
        m_keyboard = Keyboard();

    if (initFlags & InputInit::Mouse)
        m_mouse = Mouse();
}

void sdgl::InputManager::preProcessInput()
{
    if (m_keyboard)
        m_keyboard->preProcessInput();

    if (m_mouse)
        m_mouse->preProcessInput();
}
