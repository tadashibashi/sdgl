#pragma once
#include "../sdglib.h"

namespace sdgl {
    struct GamepadBtn {
        enum Enum : int {
            A = 0,
            B,
            X,
            Y,
            BumperLeft,
            BumperRight,
            StickLeft,
            StickRight,
            Back,
            Start,
            Guide,
            Left,
            Right,
            Up,
            Down,

            Count
        };

        static string_view getName(Enum btn);
    };

    struct GamepadAxis {
        enum Enum : int {
            LeftX = 0,
            LeftY,
            RightX,
            RightY,
            TriggerLeft,
            TriggerRight,

            Count
        };

        static string_view getName(Enum axis);
    };
}

