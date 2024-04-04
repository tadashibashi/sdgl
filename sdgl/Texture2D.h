#pragma once
#include "sdglib.h"
#include "math/Vector2.h"

namespace sdgl {
    struct TextureFilter
    {
        enum Enum
        {
            Nearest,
            Bilinear,
        };
    };

    /**
     * Note: loads images with uv coords where {0, 0} is on the top-left corner
     */
    class Texture2D
    {
    public:
        Texture2D(uint id, int width, int height) : m_id(id), m_size(width, height) { }
        Texture2D() : m_id(), m_size() { }

        bool loadFile(const string &filepath, TextureFilter::Enum filter = TextureFilter::Nearest);
        bool load(const string &buffer, TextureFilter::Enum filter = TextureFilter::Nearest);

        [[nodiscard]]
        auto id() const { return m_id; }
        [[nodiscard]]
        auto size() const { return m_size; }

        /**
         * Make sure to manually call this. The destructor does not do this for you.
         */
        void free();

    private:
        uint m_id;
        Point m_size;
    };
}
