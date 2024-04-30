#include "../../Keyboard.h"
#include "Keyboard.h"

#include <SDL_keycode.h>

static uint16_t s_keyToSdlKey[] = {
    SDL_SCANCODE_ESCAPE,
    SDL_SCANCODE_RETURN,
    SDL_SCANCODE_TAB,
    SDL_SCANCODE_SPACE,
    SDL_SCANCODE_BACKSPACE,
    SDL_SCANCODE_UP,
    SDL_SCANCODE_DOWN,
    SDL_SCANCODE_LEFT,
    SDL_SCANCODE_RIGHT,
    SDL_SCANCODE_INSERT,
    SDL_SCANCODE_DELETE,
    SDL_SCANCODE_HOME,
    SDL_SCANCODE_END,
    SDL_SCANCODE_PAGEUP,
    SDL_SCANCODE_PAGEDOWN,
    SDL_SCANCODE_PRINTSCREEN,
    SDL_SCANCODE_EQUALS,
    SDL_SCANCODE_MINUS,
    SDL_SCANCODE_LEFTBRACKET,
    SDL_SCANCODE_RIGHTBRACKET,
    SDL_SCANCODE_SEMICOLON,
    SDL_SCANCODE_APOSTROPHE,
    SDL_SCANCODE_COMMA,
    SDL_SCANCODE_PERIOD,
    SDL_SCANCODE_SLASH,
    SDL_SCANCODE_BACKSLASH,
    SDL_SCANCODE_GRAVE,
    SDL_SCANCODE_CAPSLOCK,

    SDL_SCANCODE_F1,
    SDL_SCANCODE_F2,
    SDL_SCANCODE_F3,
    SDL_SCANCODE_F4,
    SDL_SCANCODE_F5,
    SDL_SCANCODE_F6,
    SDL_SCANCODE_F7,
    SDL_SCANCODE_F8,
    SDL_SCANCODE_F9,
    SDL_SCANCODE_F10,
    SDL_SCANCODE_F11,
    SDL_SCANCODE_F12,

    SDL_SCANCODE_LSHIFT,
    SDL_SCANCODE_RSHIFT,
    SDL_SCANCODE_LALT,
    SDL_SCANCODE_RALT,
    SDL_SCANCODE_LCTRL,
    SDL_SCANCODE_RCTRL,
    SDL_SCANCODE_LGUI,
    SDL_SCANCODE_RGUI,

    SDL_SCANCODE_KP_0,
    SDL_SCANCODE_KP_1,
    SDL_SCANCODE_KP_2,
    SDL_SCANCODE_KP_3,
    SDL_SCANCODE_KP_4,
    SDL_SCANCODE_KP_5,
    SDL_SCANCODE_KP_6,
    SDL_SCANCODE_KP_7,
    SDL_SCANCODE_KP_8,
    SDL_SCANCODE_KP_9,

    SDL_SCANCODE_KP_PLUS,
    SDL_SCANCODE_KP_MINUS,
    SDL_SCANCODE_KP_MULTIPLY,
    SDL_SCANCODE_KP_DIVIDE,
    SDL_SCANCODE_KP_DECIMAL,
    SDL_SCANCODE_KP_ENTER,

    SDL_SCANCODE_0,
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_4,
    SDL_SCANCODE_5,
    SDL_SCANCODE_6,
    SDL_SCANCODE_7,
    SDL_SCANCODE_8,
    SDL_SCANCODE_9,

    SDL_SCANCODE_A,
    SDL_SCANCODE_B,
    SDL_SCANCODE_C,
    SDL_SCANCODE_D,
    SDL_SCANCODE_E,
    SDL_SCANCODE_F,
    SDL_SCANCODE_G,
    SDL_SCANCODE_H,
    SDL_SCANCODE_I,
    SDL_SCANCODE_J,
    SDL_SCANCODE_K,
    SDL_SCANCODE_L,
    SDL_SCANCODE_M,
    SDL_SCANCODE_N,
    SDL_SCANCODE_O,
    SDL_SCANCODE_P,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_R,
    SDL_SCANCODE_S,
    SDL_SCANCODE_T,
    SDL_SCANCODE_U,
    SDL_SCANCODE_V,
    SDL_SCANCODE_W,
    SDL_SCANCODE_X,
    SDL_SCANCODE_Y,
    SDL_SCANCODE_Z,
};

static uint16_t s_SdlKeyToKey[SDL_NUM_SCANCODES] = {0};

static_assert(sizeof(s_keyToSdlKey) / sizeof(uint16_t) == sdgl::Key::Count);

sdgl::Keyboard::Keyboard() : m_data(Key::Count * 3), m_current()
{
    if (s_SdlKeyToKey[SDL_SCANCODE_Z] == 0)
    {
        // setup sdl key to key array
        for (int i = 0; auto key : s_keyToSdlKey)
        {
            s_SdlKeyToKey[key] = i++;
        }
    }
}

bool sdgl::Keyboard::isDown(Key::Enum key) const
{
    return m_data.get(key * 3);
}

bool sdgl::Keyboard::isPressed(Key::Enum key) const
{
    const auto index = key * 3;
    return m_data.get(index) && !m_data.get(index + 1);
}

bool sdgl::Keyboard::isUp(Key::Enum key) const
{
    return !m_data.get(key * 3);
}

bool sdgl::Keyboard::isReleased(Key::Enum key) const
{
    const auto index = key * 3;
    return !m_data.get(index) && m_data.get(index + 1);
}

void sdgl::Keyboard::preProcessInput()
{
    for (size_t i = 0; i < Key::Count; ++i)
    {
        auto index = i * 3;

        // update last states
        m_data.set(index + 1, m_data.get(index));

        // process releases
        if (m_data.get(index + 2))
        {
            m_data.set(index, false);
            m_data.set(index + 2, false);
        }
    }
}

void sdgl::Keyboard::doKeyDown(unsigned int key)
{
    // exhibit "sticky" behavior of glfw3
    // layout of m_data => { value0, last0, released0, value1, last1, released1, etc...}

    const auto key = s_SdlKeyToKey[key] * 3;
    m_data.set(key, true);      // set state flag
    m_data.set(key + 2, false); // unset release flag
}

void sdgl::Keyboard::doKeyUp(unsigned int key)
{
    const auto key = s_SdlKeyToKey[key] * 3;
    m_data.set(key + 2, true); // set release flag
}
