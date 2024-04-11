#pragma once
#include <sdgl/sdglib.h>
#include <sdgl/math/Vector2.h>

#include "Color.h"

namespace sdgl {
    struct TextureFilter
    {
        enum Enum
        {
            Nearest,  ///< rounds to nearest pixel; crisp, pixellated appearance
            Bilinear, ///< bilinear interpolation between colors; softer edges by blurring
        };
    };

    /// Container for a 2D hardware texture
    /// @note non-RAII, make sure to call free once you are done using texture;
    /// @note loads images with uv coords where {0, 0} is on the top-left corner
    class Texture2D
    {
    public:
        Texture2D(uint id, int width, int height) : m_id(id), m_size(width, height) { }
        Texture2D() : m_id(), m_size() { }

        bool loadFile(const string &filepath, TextureFilter::Enum filter = TextureFilter::Nearest);
        bool load(const string &buffer, TextureFilter::Enum filter = TextureFilter::Nearest);

        /// Load texture data from format RGBA8888
        bool loadBytes(const void *data, size_t length, int width, int height, TextureFilter::Enum filter);
        bool loadBytes(string_view buffer, int width, int height, TextureFilter::Enum filter);
        bool loadBytes(const vector<Color> &pixels, int width, int height, TextureFilter::Enum filter);

        /// Graphics library texture id, castable to ImGui image type
        [[nodiscard]]
        auto id() const { return m_id; }

        /// Dimensions of the image in pixels
        [[nodiscard]]
        auto size() const { return m_size; }

        /// This should be manually called, as the destructor will not call it
        void free();

    private:
        uint m_id;
        Point m_size;
    };
}
