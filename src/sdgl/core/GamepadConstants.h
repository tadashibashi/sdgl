#pragma once
#include "../sdglib.h"

namespace sdgl {
    struct GamepadBtn {
        enum Enum : int {
            A = 0,       ///< Bottom of ABXY buttons
            B,           ///< Right of ABXY buttons
            X,           ///< Left of ABXY buttons
            Y,           ///< Top of ABXY buttons
            BumperLeft,  ///< Left trigger
            BumperRight, ///< Right trigger
            StickLeft,   ///< Left stick when pressed down (not axis)
            StickRight,  ///< Right stick pressed down, often used in fps games for activating scope
            Back,        ///< UI go back
            Start,       ///< UI progress forward
            Guide,       ///< System menu button
            Left,        ///< D-pad left
            Right,       ///< D-pad right
            Up,          ///< D-pad up
            Down,        ///< D-pad down

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

