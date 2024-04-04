#pragma once
#include "Color.h"
#include "Texture2D.h"
#include "math/Rectangle.h"
#include "math/Vector2.h"

namespace sdgl {
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

    class SpriteBatch {
        struct Vertex
        {
            Vertex() : position{}, color{}, uv{} { }
            Vertex(const Vector2 position, const Color color, const Vector2 uv) :
                position{position}, color{color}, uv{uv} {}

            Vector2  position;
            Color    color;
            Vector2  uv;
        };

        struct Glyph
        {
            Vertex topleft, bottomleft, topright, bottomright;
            Texture2D texture{};
            float depth{};
        };

        struct RenderBatch
        {
            RenderBatch(const uint offset, const uint vertexCount, const uint texture)
                    : offset(offset), vertexCount(vertexCount), texture(texture)
            {}
            uint offset;
            uint vertexCount;
            uint texture;
        };
    public:
        SpriteBatch() : m_glyphs{}, m_batches{}, m_shader{}, m_sortOrder{SortOrder::BackToFront},
            m_vbo{}, m_vao{} { }
        ~SpriteBatch() = default;

        void drawTexture(Texture2D texture, Rectangle source, Vector2 position, Color color, Vector2 scale, Vector2 anchor, float angle, float depth);
        void begin(const float *transformMatrix, SortOrder::Enum sortOrder);
        void end();

    private:
        vector<Glyph> m_glyphs;
        vector<RenderBatch> m_batches;

        Shader *m_shader;
        SortOrder::Enum m_sortOrder;
        uint m_vbo, m_vao;

        void createBatches();
        void sortGlyphs();
        void createVertexArray();
        void disableVertexArray();
    };
}
