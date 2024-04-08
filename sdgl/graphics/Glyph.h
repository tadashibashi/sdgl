#pragma once
#include <sdgl/sdglib.h>
#include <sdgl/math/Rectangle.h>
#include <sdgl/graphics/Texture2D.h>

namespace sdgl::graphics {

    /// Instructions for rendering a text glyph
    struct Glyph
    {
        Glyph(const Rectangle_<uint16> frame, const Point destination, const Texture2D texture) :
            frame(frame), destination(destination), texture(texture) { }

        Rectangle_<uint16> frame;  ///< Source frame rectangle in pixels within texture
        Point destination;         ///< Destination position, relative to {0, 0}
        Texture2D texture;            ///< Gpu texture id (necessary to support multiple-paged fonts)
    };
}
