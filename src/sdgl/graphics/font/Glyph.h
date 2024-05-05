#pragma once
#include <sdgl/sdglib.h>
#include <sdgl/graphics/Texture2D.h>
#include <sdgl/math/Rectangle.h>

namespace sdgl {
    struct Frame;

    /// Instructions for rendering a text glyph
    /// This may be usable for any texture, but does not contain offset information to support trimming and rotation
    /// since bitmap fonts don't trim or rotate locally.
    struct Glyph
    {
        Glyph(const Rect<uint16> frame, const Point destination, const Frame &parentFrame) :
            source(frame), destination(destination), frame(parentFrame) { }

        Rect<int16> source;  ///< Source frame rectangle in pixels within frame
        Point destination;   ///< Destination position, relative to {0, 0}
        const Frame &frame;  ///< Source atlas texture frame
    };
}
