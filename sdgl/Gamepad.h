#pragma once

namespace sdgl {
    struct GamepadBtn {
        enum Enum {
            A,
            B,
            X,
            Y,
            BumperLeft,
            BumperRight,
            Back,
            Start,
            Guide,
            Left,
            Right,
            Up,
            Down,

            Count
        };
    };

    struct GamepadAxis {
        enum Enum {
            LeftX,
            LeftY,
            RightX,
            RightY,
            TriggerLeft,
            TriggerRight,

            Count
        };
    };
}