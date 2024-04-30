#pragma once
#include <sdgl/math/Rectangle.h>
#include "Texture2D.h"

namespace sdgl {
    struct Frame {
        Rect<int16> frame{};
        Vec2<int16> offset{};
        Vec2<int16> size{};
        bool rotated = false;
        Texture2D texture{};
    };
}
