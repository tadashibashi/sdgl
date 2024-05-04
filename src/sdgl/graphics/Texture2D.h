#pragma once
#include <sdgl/Asset.h>
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
    class Texture2D final : public Asset
    {
    public:
        /// Emplace a GL texture that has already been created into a Texture2D
        /// @param id     OpenGL texture id
        /// @param width  pixel width
        /// @param height pixel height
        Texture2D(uint id, int width, int height) : m_id(id), m_size(width, height) { }

        /// Create an unloaded texture
        Texture2D() : m_id(), m_size() { }

        /// Doesn't clean up texture id - make sure to call unload manually
        ~Texture2D() override = default;

        bool loadFile(const string &filepath, TextureFilter::Enum filter = getDefaultFilter());
        bool loadMem(const string &buffer, TextureFilter::Enum filter = getDefaultFilter());

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
        void unload() override;

        static void setDefaultFilter(TextureFilter::Enum filterType) { s_defaultFilter = filterType; }
        static TextureFilter::Enum getDefaultFilter() { return s_defaultFilter; }
    private:
        static TextureFilter::Enum s_defaultFilter;

        uint m_id;
        Point m_size;
    };
}
