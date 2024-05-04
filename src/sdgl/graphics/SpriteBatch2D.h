///
/// Future features:
/// - Basic primitives (shader-based or from triangles?)
/// - Inject shader code for fx
///
#pragma once
#include <sdgl/graphics/Color.h>
#include <sdgl/graphics/Texture2D.h>
#include <sdgl/math/Rectangle.h>
#include <sdgl/math/Vector2.h>

#include "RenderProgram.h"

namespace sdgl {
    class Camera2D;
    class FontText;
    class Frame;
    class Shader;

    struct SortOrder {
        enum Enum
        {
            None,
            FrontToBack,
            BackToFront,
            Texture
        };
    };

    /// Basic sprite batch for rendering texture quads.
    /// Intended for use with a texture atlas for efficient rendering.
    class SpriteBatch2D {
        struct Vertex
        {
            Vertex() : position{}, color{}, texcoord{} { }
            Vertex(const Vector2 position, const Color color, const Vector2 uv) :
                position{position}, color{color}, texcoord{uv} {}

            Vector2  position;
            Vector2  texcoord;
            Color    color;
        };

        struct Glyph
        {
            Vertex topleft, bottomleft, topright, bottomright;
            Texture2D texture{};
            float depth{};
        };

        struct RenderBatch
        {
            RenderBatch(const uint offset, const uint vertexCount, const Texture2D &texture)
                    : offset(offset), count(vertexCount), texture(texture)
            {}
            uint offset;        ///< starting index in the vertex array
            uint count;         ///< number of objects in this batch
            Texture2D texture;  ///< texture to render for this batch
        };
    public:
        SpriteBatch2D();
        ~SpriteBatch2D() = default;

        virtual void init();

        /// Draw a subimage of a texture
        void drawTexture(
            const Texture2D &texture, ///< texture to draw
            Rectangle source,         ///< source rectangle within the texture in pixels
            Vector2 position,         ///< position in pixels at which to project image
            Color tint = Color::White,   ///< color to tint the image
            Vector2 scale = {1.f, 1.f},   ///< normalized texture xy scale
            Vector2 anchor = {0, 0},      ///< the (0, 0) position within the texture from which to rotate / scale from (in pixels)
            float angle = 0,              ///< rotation in radians
            float depth = 0               ///< depth sorting value (set sortOrder in `SpriteBatch::begin` to set behavior)
        );


        /// FIXME: perhaps cache rendered texture inside of FontText component? This would allow for ease of
        /// transformations using drawTexture
        void drawText(const FontText &text, Vector2 position, Color color = Color::White, float depth = 0);

        void drawFrame(const Frame &frame, Vector2 position, Color color, Vector2 scale, Vector2 anchor, float angle, float depth);

        void begin(const float *transformMatrix, SortOrder::Enum sortOrder = SortOrder::FrontToBack);

        /// End sprite batch drawing; must be paired with begin()
        void end();

    private:


        vector<Glyph> m_glyphs;
        vector<RenderBatch> m_batches;

        SortOrder::Enum m_sortOrder;
        RenderProgram m_program;

        bool m_batchStarted;

        int u_texture, u_projMtx, u_texSize;

        const float *m_matrix;


        void createBatches();
        void sortGlyphs();
        void renderBatches();
    };
}
