#include "SpriteBatch.h"
#include <sdgl/math/geometry.h>

namespace sdgl {
    void SpriteBatch::drawTexture(Texture2D texture, Rectangle source, Vector2 position, Color color, Vector2 scale,
        Vector2 anchor, float angle, float depth)
    {
        // Texture checks
        SDGL_ASSERT(texture.id());
        SDGL_ASSERT(texture.size().x > 0 && texture.size().y > 0);

        // Get source uv from pixel coords (normalized texture positions)
        const auto textureSize = Vector2(texture.size());
        const FRectangle uv = {
            static_cast<float>(source.x) / textureSize.x, // x
            static_cast<float>(source.y) / textureSize.y, // y
            static_cast<float>(source.w) / textureSize.x, // width
            static_cast<float>(source.h) / textureSize.y  // height
        };

        // Snap destination coords to pixel grid
        FRectangle dest = {
            std::round(position.x - anchor.x*scale.x),
            std::round(position.y - anchor.y*scale.y),
            std::round(static_cast<float>(source.w) * scale.x), // width
            std::round(static_cast<float>(source.h) * scale.y)  // height
        };

        // Scale anchor point
        anchor *= scale;

        // Calculate destination offset from rotation & anchor point (get zero-ed point, rotate it, then reapply
        //auto offsetTopLeft     = mathf::rotate( Vector2(-anchor.x, -anchor.y), angle ) + anchor;
        //auto offsetBottomLeft  = mathf::rotate( Vector2(-anchor.x, -dest.h - anchor.y) )



        Glyph glyph;
        glyph.texture = texture;
        glyph.depth = depth;



    }
}
