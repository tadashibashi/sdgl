#include "SpriteBatch2D.h"
#include <sdgl/math/geometry.h>
#include "detail/spriteBatch2DShader.inl"
#include <sdgl/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace sdgl::graphics {
    static constexpr int VertsPerQuad = 6;

    SpriteBatch2D::SpriteBatch2D() :
        m_glyphs(), m_batches(),
        m_sortOrder(SortOrder::BackToFront),
        m_program(), m_batchStarted(false), u_texture(),
        u_projMtx(), u_texSize(), m_matrix()
    { }

    void SpriteBatch2D::init()
    {
        // initialize program
        m_program.init({
            .vertShader = detail::spriteBatch2dVertShader,
            .fragShader = detail::spriteBatch2dFragShader,
            .attributes = ShaderAttribs()
                .attrib(&Vertex::position, GLType::Float, 2)
                .attrib(&Vertex::texcoord, GLType::Float, 2)
                .attrib(&Vertex::color, GLType::Ubyte, 4, true),
            .openFiles = false,
        });

        // locate shader uniforms
        u_texture = m_program.shader()->locateUniform("u_Texture");
        u_projMtx = m_program.shader()->locateUniform("u_ProjMtx");
        u_texSize = m_program.shader()->locateUniform("u_TexSize");
    }

    void SpriteBatch2D::drawTexture(
        const Texture2D &texture,
        Rectangle source,
        Vector2 position,
        Color color,
        Vector2 scale,
        Vector2 anchor,
        float angle,
        float depth
    )
    {
        // Texture checks
        SDGL_ASSERT(texture.id());
        SDGL_ASSERT(texture.size().x > 0 && texture.size().y > 0);

        const auto texCoords = static_cast<FRectangle>(source);

        // Scale anchor point
        anchor *= scale;
        
        // Snap destination coords to pixel grid
        FRectangle dest = {
            std::round(position.x),
            std::round(position.y),
            std::round(texCoords.w * scale.x), // width
            std::round(texCoords.h * scale.y)  // height
        };

        // Calculate destination offset from rotation & anchor point (get zero-ed point, rotate it, then reapply
        auto offsetTopLeft     = mathf::rotate(-anchor, angle);
        auto offsetBottomLeft  = mathf::rotate(Vector2(-anchor.x, dest.h - anchor.y), angle);
        auto offsetTopRight    = mathf::rotate(Vector2(dest.w - anchor.x, -anchor.y), angle);
        auto offsetBottomRight = mathf::rotate(Vector2(dest.w - anchor.x, dest.h - anchor.y), angle);

        // Create and append glyph/quad image
        Glyph glyph;
        glyph.texture = texture;
        glyph.depth = depth;

        glyph.topleft.color = color;
        glyph.topleft.position = Vector2(dest.x + offsetTopLeft.x, dest.y + offsetTopLeft.y);;
        glyph.topleft.texcoord = texCoords.topleft();

        glyph.bottomleft.color = color;
        glyph.bottomleft.position = Vector2(dest.x + offsetBottomLeft.x, dest.y + offsetBottomLeft.y);
        glyph.bottomleft.texcoord = texCoords.bottomleft();

        glyph.bottomright.color = color;
        glyph.bottomright.position = Vector2(dest.x + offsetBottomRight.x, dest.y + offsetBottomRight.y);
        glyph.bottomright.texcoord = texCoords.bottomright();

        glyph.topright.color = color;
        glyph.topright.position = Vector2(dest.x + offsetTopRight.x, dest.y + offsetTopRight.y);
        glyph.topright.texcoord = texCoords.topright();

        m_glyphs.emplace_back(glyph);
    }

    void SpriteBatch2D::begin(const float *transformMatrix, const SortOrder::Enum sortOrder)
    {
        m_matrix = transformMatrix;
        m_batches.clear();
        m_glyphs.clear();
        m_sortOrder = sortOrder;
    }

    void SpriteBatch2D::end()
    {
        sortGlyphs();
        createBatches();
        renderBatches();
    }


    void SpriteBatch2D::createBatches()
    {
        if (m_glyphs.empty())
            return;

        // FUTURE: Some research should be done to see when index vs vertex rendering is optimal.
        if (m_glyphs.size() > 10000u) // ----- Index Mode: draw using vertex indices -----
        {
            vector<uint> indices;
            vector<Vertex> vertices;
            vector<RenderBatch> batches;
            vertices.reserve(m_glyphs.size() * 4);
            indices.reserve(m_glyphs.size() * VertsPerQuad);

            for (uint indexOffset = 0, lastTexId = UINT32_MAX;
                const auto &[topleft, bottomleft,topright, bottomright, texture, depth] : m_glyphs)
            {
                // Each texture swap requires a new batch
                if (const auto texId = texture.id();
                    lastTexId != texId)
                {
                    batches.emplace_back(indexOffset, VertsPerQuad, texture);
                    lastTexId = texId;
                }
                else
                {
                    // same texture - stay on same batch - add num indices for this glyph
                    batches.back().count += VertsPerQuad;
                }

                auto vertIndex = vertices.size(); // first vertex index for this quad

                // add quad vertices (only 4 needed, since we're pointing to them via indices)
                vertices.emplace_back(topleft);
                vertices.emplace_back(bottomleft);
                vertices.emplace_back(bottomright);
                vertices.emplace_back(topright);

                // add quad vertex indices
                indices.emplace_back(vertIndex);
                indices.emplace_back(vertIndex + 1);
                indices.emplace_back(vertIndex + 2);
                indices.emplace_back(vertIndex + 2);
                indices.emplace_back(vertIndex + 3);
                indices.emplace_back(vertIndex);
                indexOffset += VertsPerQuad;
            }

            // send vertices to the graphics card
            m_program.setVertices(vertices);
            m_program.setIndices(indices);

            // done, commit batches
            m_batches.swap(batches);
        }
        else                         // ----- Vertex Mode: draw individual vertices -----
        {
            vector<Vertex> vertices;
            vector<RenderBatch> batches;
            vertices.reserve(m_glyphs.size() * VertsPerQuad);

            for (uint offset = 0, lastTexId = UINT32_MAX;
                const auto &[topleft, bottomleft, topright, bottomright, texture, depth] : m_glyphs)
            {
                // Each texture swap requires a new batch
                if (const auto texId = texture.id(); lastTexId != texId)
                {
                    batches.emplace_back(offset, VertsPerQuad, texture);
                    lastTexId = texId;
                }
                else
                {
                    // same texture - stay on same batch - add num vertices to batch for this glyph
                    batches.back().count += VertsPerQuad;
                }

                // add vertices for this quad
                vertices.emplace_back(topleft);
                vertices.emplace_back(bottomleft);
                vertices.emplace_back(bottomright);
                vertices.emplace_back(bottomright);
                vertices.emplace_back(topright);
                vertices.emplace_back(topleft);

                offset += VertsPerQuad;
            }

            // send vertices to graphics card
            m_program.setVertices(vertices, true);
            m_program.clearIndices(); // turn off index mode, in case it was on previously

            // done, commit batches
            m_batches.swap(batches);
        }
    }

    void SpriteBatch2D::sortGlyphs()
    {
        switch(m_sortOrder)
        {
            case SortOrder::BackToFront:
                std::ranges::stable_sort(m_glyphs, [](const Glyph &a, const Glyph &b) {
                    return a.depth > b.depth;
                });
            break;

            case SortOrder::FrontToBack:
                std::ranges::stable_sort(m_glyphs, [](const Glyph &a, const Glyph &b) {
                    return a.depth < b.depth;
                });
            break;

            case SortOrder::Texture:
                std::ranges::stable_sort(m_glyphs, [](const Glyph &a, const Glyph &b) {
                    return a.texture.id() < b.texture.id();
                });
            break;

            case SortOrder::None:
            break;
        }
    }

    void SpriteBatch2D::renderBatches()
    {
        static glm::mat4 defaultMatrix;

        // Set projection matrix
        m_program.shader()->setUniformMatrix(u_projMtx, m_matrix ? m_matrix : &defaultMatrix[0][0]);

        // Render each batch
        for (const auto &batch : m_batches)
        {
            // Set texture data
            const auto texSize = Vector2(batch.texture.size());
            m_program.shader()->setUniform(u_texSize, texSize);
            m_program.shader()->setUniform(u_texture, batch.texture);

            // Draw batch
            m_program.render(PrimitiveType::Triangles,
              static_cast<GLint>(batch.offset),
              static_cast<GLint>(batch.count));
        }
    }
}