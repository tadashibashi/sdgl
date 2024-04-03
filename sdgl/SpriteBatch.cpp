//
// Created by Aaron Ishibashi on 4/2/24.
//

#include "SpriteBatch.h"

namespace sdgl {
    void SpriteBatch::drawTexture(Texture2D texture, Rectangle source, Vector2 position, Color color, Vector2 scale,
        Vector2 anchor, float rotation, float depth)
    {
        Glyph glyph;
        glyph.texture = texture;
        glyph.depth = depth;

        FRectangle dest;


    }
}
