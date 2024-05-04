#include "SpriteBatch2D.h"
#include "spriteBatch2DShader.inl"

#include <sdgl/math/geometry.h>

#include <sdgl/graphics/font/Glyph.h>
#include <sdgl/graphics/font/FontText.h>

#include <glm/gtc/type_ptr.hpp>

namespace sdgl {
    static constexpr int VertsPerQuad = 6;

    SpriteBatch2D::SpriteBatch2D() :
        m_glyphs(), m_batches(),
        m_sortOrder(SortOrder::BackToFront),
        m_program(), m_batchStarted(false), u_texture(),
        u_projMtx(), u_texSize(), m_matrix()
    {
    }

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
        SDGL_ASSERT(texture.id(), "Texture must be initialized");

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

        // Calculate destination offset from rotation & anchor point (get zero-ed point, rotate it, then add to glyph points for total position)
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

    void SpriteBatch2D::drawText(const FontText &text, const Vector2 position, const Color color, float depth)
    {
        for (size_t i = 0; const auto &glyph : text.glyphs())
        {
            if (i > text.textProgress())
                break;

            Rectangle source;
            float rotation = 0;
            Vector2 anchor;

            auto &texFrame = glyph.frame;

            // transpose glyph source within font to position in the wider atlas
            if (texFrame.rotated)
            {
                source = {
                    static_cast<int16>((int16)texFrame.frame.w + texFrame.offset.y  + texFrame.frame.x - glyph.source.y - glyph.source.h),
                    static_cast<int16>(glyph.source.x + texFrame.frame.y + texFrame.offset.x),
                    glyph.source.h,
                    glyph.source.w
                };
                anchor = Vector2{(float)glyph.source.h - (float)texFrame.offset.y, (float)texFrame.offset.x};
                rotation = -mathf::HalfPi;
            }
            else
            {
                source = {
                    static_cast<int16>(glyph.source.x + texFrame.frame.x),
                    static_cast<int16>(glyph.source.y + texFrame.frame.y),
                    glyph.source.w,
                    glyph.source.h
                };

                anchor += texFrame.offset;
            }

            drawTexture(glyph.frame.texture, source, position + glyph.destination, color, {1.f, 1.f}, anchor, rotation, depth);
            ++i;
        }
    }

    void SpriteBatch2D::drawFrame(const Frame &frame, Vector2 position, Color color, Vector2 scale,
        Vector2 anchor, float angle, float depth)
    {
        if (frame.rotated)
        {
            angle -= mathf::HalfPi;
            anchor = Vector2{frame.frame.w - frame.offset.y - anchor.y, frame.offset.x + anchor.x};
        }
        else
        {
            anchor += frame.offset;
        }

        drawTexture(frame.texture, (Rectangle)frame.frame, position, color, scale, anchor, angle, depth);
    }

    void SpriteBatch2D::begin(const float *transformMatrix, const SortOrder::Enum sortOrder)
    {
        SDGL_ASSERT(!m_batchStarted,
            "Mismatched SpriteBatch2D::begin call. Did you remember to call end?");
        m_matrix = transformMatrix;
        m_batches.clear();
        m_glyphs.clear();
        m_sortOrder = sortOrder;

        m_batchStarted = true;
    }

    void SpriteBatch2D::end()
    {
        SDGL_ASSERT(m_batchStarted,
            "Mismatched SpriteBatch2D::end call. Did you remember to call begin?");

        sortGlyphs();
        createBatches();
        renderBatches();

        m_batchStarted = false;
    }


    void SpriteBatch2D::createBatches()
    {
        if (m_glyphs.empty())
            return;

        // FUTURE: Some research should be done to see when index vs vertex rendering is optimal.
        if (false) // ----- Index Mode: draw using vertex indices -----
        {
            vector<uint> indices;
            vector<Vertex> vertices;
            auto &batches = m_batches;
            batches.clear();

            vertices.reserve(m_glyphs.size() * 4);
            indices.reserve(m_glyphs.size() * VertsPerQuad);

            for (uint indexOffset = 0, vertOffset = 0, lastTexId = UINT32_MAX;
                const auto &[topleft, bottomleft, topright, bottomright, texture, depth] : m_glyphs)
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

                // add quad vertices (only 4 needed, since we're pointing to them via indices)
                vertices.emplace_back(topleft);
                vertices.emplace_back(bottomleft);
                vertices.emplace_back(bottomright);
                vertices.emplace_back(topright);


                // add quad vertex indices
                indices.emplace_back(vertOffset);
                indices.emplace_back(vertOffset + 1);
                indices.emplace_back(vertOffset + 2);
                indices.emplace_back(vertOffset + 2);
                indices.emplace_back(vertOffset + 3);
                indices.emplace_back(vertOffset);
                indexOffset += VertsPerQuad;
                vertOffset += 4;
            }

            // send vertices to the graphics card
            m_program.setIndices(indices, true);
            m_program.setVertices(vertices, true);
        }
        else                         // ----- Vertex Mode: draw individual vertices -----
        {
            vector<Vertex> vertices;
            auto &batches = m_batches;
            batches.clear();

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
        static glm::mat4 defaultMatrix{1.f};

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
              static_cast<int>(batch.offset),
              static_cast<int>(batch.count));
        }
    }

} // namespace sdgl
