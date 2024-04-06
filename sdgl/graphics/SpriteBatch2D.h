///
/// Future features:
/// - Basic primitives (graphics card-based or from triangles?)
/// - Inject shader code for fx
///
#pragma once
#include <sdgl/Color.h>
#include <sdgl/Texture2D.h>
#include <sdgl/math/Rectangle.h>
#include <sdgl/math/Vector2.h>

#include "RenderProgram.h"

namespace sdgl {
    class Camera2D;
    class Shader;
}

namespace sdgl::graphics {

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
            Color    color;
            Vector2  texcoord;
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

        void init();

        void drawTexture(const Texture2D &texture, Rectangle source, Vector2 position, Color color, Vector2 scale, Vector2 anchor, float angle, float depth);

        void begin(const float *transformMatrix, SortOrder::Enum sortOrder = SortOrder::FrontToBack);

        void end();

    private:
        vector<Glyph> m_glyphs;
        vector<RenderBatch> m_batches;

        SortOrder::Enum m_sortOrder;
        graphics::RenderProgram m_program;

        int u_texture, u_projMtx, u_texSize;

        const float *m_matrix;

        void createBatches();
        void sortGlyphs();
        void renderBatches();
    };
}
