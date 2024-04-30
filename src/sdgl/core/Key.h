#pragma once
#include "../sdglib.h"

namespace sdgl {

    struct ModKey {
        enum Enum : uint {
            None = 0,
            Shift = 1,
            Ctrl = 1 << 1,
            Alt = 1 << 2,
            Meta = 1 << 3,
        };
    };

    inline ModKey::Enum operator| (ModKey::Enum a, ModKey::Enum b)
    {
        return static_cast<ModKey::Enum>(static_cast<uint>(a) | static_cast<uint>(b));
    }

    struct Key
    {
        enum Enum
        {
            Escape=0,
            Return,
            Tab,
            Space,
            Backspace,
            Up,
            Down,
            Left,
            Right,
            Insert,
            Delete,
            Home,
            End,
            PageUp,
            PageDown,
            Print,
            Equal,
            Minus,
            BracketLeft,
            BracketRight,
            Semicolon,
            Quote,
            Comma,
            Period,
            Slash,
            Backslash,
            BackTick,
            CapsLock,

            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,

            ShiftLeft,
            ShiftRight,
            AltLeft,
            AltRight,
            CtrlLeft,
            CtrlRight,
            MetaLeft,
            MetaRight,

            NumPad0,
            NumPad1,
            NumPad2,
            NumPad3,
            NumPad4,
            NumPad5,
            NumPad6,
            NumPad7,
            NumPad8,
            NumPad9,

            NumPadPlus,
            NumPadMinus,
            NumPadMultiply,
            NumPadDivide,
            NumPadDecimal,
            NumPadEnter,

            Number0,
            Number1,
            Number2,
            Number3,
            Number4,
            Number5,
            Number6,
            Number7,
            Number8,
            Number9,

            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,

            Count
        };

        /// Get name for key
        static string_view getName(Key::Enum key);
    };
}
