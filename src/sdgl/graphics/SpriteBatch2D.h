#pragma once
#include "SpriteBatchBase2D.h"
#include "Texture2D.h"

namespace sdgl {
    /// Provides some convenience functions in addition to the base class
    class SpriteBatch2D : public SpriteBatchBase2D {
    public:
        void init() override;

        void drawRectangle(Rectangle rect, Color tint = Color::White,
            Vector2 scale = {1.f, 1.f}, Vector2 anchor = {0, 0},
            float angle = 0, float depth = 0);

        /// Draw entire texture
        void drawTexture(
            const Texture2D &texture,     ///< texture to draw
            Vector2 position,             ///< position to draw texture at
            Color tint = Color::White,    ///< color to tint texture with
            Vector2 scale = {1.f, 1.f},   ///< scale drawing
            Vector2 anchor = {0, 0},      ///< where the origin is located within texture, from which to draw and rotate
            float angle = 0,              ///< angle to rotate drawing in radians
            float depth = 0               ///< depth sorting value, meaning determined by sort value passed to `begin`
        );

        /// Draw texture mapped to destination rectangle
        void drawTexture(const Texture2D &texture, Rectangle source,
                            Rectangle destination, Color tint = Color::White,
                            Vector2 anchor = {0, 0}, float angle = 0,
                            float depth = 0);
    private:
        Texture2D m_pixel;
    };
}


