#include "../../Mouse.h"
#include <SDL2/SDL.h>

// static int8_t s_buttonToSdlButton[] = {
//     SDL_BUTTON_LEFT,
//     SDL_BUTTON_MIDDLE,
//     SDL_BUTTON_RIGHT,
// };

sdgl::Mouse::Mouse() : m_x(), m_y(), m_lastX(), m_lastY(),
    m_scrollX(), m_scrollY(), m_lastScrollX(), m_lastScrollY(),
    m_buttons(MouseBtn::Count), m_lastButtons(MouseBtn::Count),
    m_doubleClick(MouseBtn::Count), m_released(MouseBtn::Count)
{}

void sdgl::Mouse::getPosition(float *x, float *y) const
{
    if (x)
        *x = m_x;
    if (y)
        *y = m_y;
}

bool sdgl::Mouse::isDown(MouseBtn::Enum button) const
{
    return m_buttons.get(button);
}

bool sdgl::Mouse::isPressed(MouseBtn::Enum button) const
{
    return m_buttons.get(button) && !m_lastButtons.get(button);
}

bool sdgl::Mouse::isUp(MouseBtn::Enum button) const
{
    return !m_buttons.get(button);
}

bool sdgl::Mouse::isReleased(MouseBtn::Enum button) const
{
    return !m_buttons.get(button) && m_lastButtons.get(button);
}

bool sdgl::Mouse::isDoubleClicked(MouseBtn::Enum button) const
{
    return m_doubleClick.get(button);
}

float sdgl::Mouse::getAxis(MouseAxis::Enum axis) const
{
    switch(axis)
    {
        case MouseAxis::ScrollX:
            return m_scrollX;
        case MouseAxis::ScrollY:
            return m_scrollY;
        default:
            return 0;
    }
}

float sdgl::Mouse::getlastAxis(MouseAxis::Enum axis) const
{
    switch(axis)
    {
        case MouseAxis::ScrollX:
            return m_lastScrollX;
        case MouseAxis::ScrollY:
            return m_lastScrollY;
        default:
            return 0;
    }
}

void sdgl::Mouse::preProcessInput()
{
    m_lastX = m_x;
    m_lastY = m_y;

    m_lastScrollX = m_scrollX;
    m_lastScrollY = m_scrollY;

    m_scrollX = 0;
    m_scrollY = 0;

    const auto memSize = m_buttons.size() * sizeof(uint32_t);

    // update last states
    std::memcpy(m_lastButtons.data(), m_buttons.data(), memSize);

    // unset double clicks
    std::memset(m_doubleClick.data(), 0, memSize);

    // apply releases
    static_assert(MouseBtn::Count <= 32 && MouseBtn::Count > 0);
    {
        uint32_t *current = m_buttons.data();
        uint32_t *released = m_released.data();
        auto result = ~*current | *released;
        *current = ~result;
        *released = result;
    }
}

void sdgl::Mouse::setPosition(float x, float y)
{
    m_x = x;
    m_y = y;
}

static int8_t sdlButtonToButton(unsigned int buttonid)
{
    using sdgl::MouseBtn;

    switch(buttonid)
    {
        case SDL_BUTTON_LEFT:
            return MouseBtn::Left;
        case SDL_BUTTON_MIDDLE:
            return MouseBtn::Middle;
        case SDL_BUTTON_RIGHT:
            return MouseBtn::Right;
        default:
            return -1; // unknown
    }
}

void sdgl::Mouse::doButtonDown(uint8_t buttonid)
{
    if (const auto index = buttonid - 1; index < 3)
    {
        m_buttons.set(index, true);
        m_released.set(index, false);
    }
}

void sdgl::Mouse::doButtonUp(uint8_t buttonid)
{
    if (const auto index = buttonid - 1; index < 3)
    {
        m_released.set(index, true);
    }
}

void sdgl::Mouse::doButtonDoubleClick(uint8_t buttonid)
{
    if (const auto index = buttonid - 1; index < 3)
    {
        m_doubleClick.set(index, true);
        m_buttons.set(index, true);
        m_released.set(index, false);
    }
}
