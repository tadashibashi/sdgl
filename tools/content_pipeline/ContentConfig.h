#pragma once
#include <sdgl/sdglib.h>

namespace sdgl::content {
    ///
    /// Content types:
    /// Font
    ///     - AngelCode BMFont
    ///         - Binary font descriptor file
    ///         - PNG font image
    ///     - TTF (future via stb)
    ///         - .ttf file
    ///         - Font descriptor format
    /// Sprites
    ///    - PNG images in folders
    ///         - One JSON config mapping sprite
    ///
    ///
    struct ContentConfig {
        string fonts;         // angelcode bmfont
        string sprites;
        string backgrounds;
        string tilesets;
        string tilemaps;
    };
}
