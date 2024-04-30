#pragma once
#include "Key.h"
#include <sdgl/BitFlags.h>

namespace sdgl {
    class Keyboard {
    public:
        Keyboard();

        [[nodiscard]]
        bool isDown(Key::Enum key) const;

        [[nodiscard]]
        bool isPressed(Key::Enum key) const;

        [[nodiscard]]
        bool isUp(Key::Enum key) const;

        [[nodiscard]]
        bool isReleased(Key::Enum key) const;

        // Call this before polling for input or at the end of the main loop
        void preProcessInput();

    private:
        friend class Backend;

        /// Note to backend: only pass initial key down, not repeats
        /// @param key key code of SDL2/3
        void doKeyDown(unsigned int key);
        void doKeyUp(unsigned int key);

        BitFlags m_data;
        bool m_current;
    };
}