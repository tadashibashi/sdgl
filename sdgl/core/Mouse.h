#pragma once

namespace sdgl {
    struct MouseBtn {
        enum Enum {
            Left,
            Right,
            Middle,

            Count
        };
    };

    struct MouseAxis {
        enum Enum {
            ScrollX,
            ScrollY,

            Count
        };
    };
}