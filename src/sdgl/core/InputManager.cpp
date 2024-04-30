#include "InputManager.h"

sdgl::InputManager::InputManager() : m_keyboard()
{

}

void sdgl::InputManager::init(InputInit::Flags initFlags)
{
    if (initFlags & InputInit::Keyboard)
    {
        m_keyboard = Keyboard();
    }
}

void sdgl::InputManager::preProcessInput()
{

}
