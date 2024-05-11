#include "SpriteBatch2D.h"

namespace sdgl {
    void SpriteBatch2D::init()
    {
        m_pixel.loadBytes({Color::White}, 1, 1, TextureFilter::Nearest);
        SpriteBatchBase2D::init();
    }

    void SpriteBatch2D::drawTexture(const Texture2D &texture, Rectangle source,
                                Rectangle destination, Color tint,
                                Vector2 anchor, float angle, float depth)
    {
        if (source.w == 0 || source.h == 0) // avoid div by zero
            return;
        auto scale = Vector2((float)destination.w / source.w,
                                (float)destination.h / source.h);
        SpriteBatchBase2D::drawTexture(texture, source,
                    {(float)destination.x, (float)destination.y}, tint, scale,
                    anchor, angle, depth);
    }

    void SpriteBatch2D::drawRectangle(Rectangle rect, Color tint, Vector2 scale, Vector2 anchor, float angle, float depth)
    {
        rect.w = (int)((float)rect.w * scale.x);
        rect.h = (int)((float)rect.h * scale.y);
        anchor *= scale;
        drawTexture(m_pixel, {0, 0, 1, 1}, rect, tint, anchor, angle, depth);
    }

    void SpriteBatch2D::drawTexture(const Texture2D &texture, Vector2 position,
                                  Color tint, Vector2 scale, Vector2 anchor,
                                  float angle, float depth)
    {
        const auto texSize = texture.size();
        SpriteBatchBase2D::drawTexture(texture, {0, 0, texSize.x, texSize.y}, position, tint,
                    scale, anchor, angle, depth);
    }
}