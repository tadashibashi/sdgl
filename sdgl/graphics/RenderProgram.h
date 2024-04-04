#pragma once
#include <sdgl/Shader.h>
#include <sdgl/ShaderAttribs.h>

namespace sdgl::graphics {
    struct PrimitiveType
    {
        enum Enum
        {
            Points = 0,
            Lines,
            LineStrip,
            LineLoop,
            Triangles,
            TriangleStrip,
            TriangleFan,
        };
    };
    ///
    /// Straightforward, single-buffer shader program
    ///
    class RenderProgram {
    public:
        struct Config
        {
            /// Shader string, if `openFiles` is true, this is a file path to a shader file
            string vertShader, fragShader;

            /// Attribute details relating a shader to its accompanying vertex buffer
            ShaderAttribs attributes;

            /// Whether `vertShaer` & `fragShader` values are filepaths to be opened.
            /// If false, these fields contain code strings.
            bool openFiles;
        };

        explicit RenderProgram(Config config);
        RenderProgram();

        /// Initialize the program, this must be called before `render`
        /// @returns whether initialization succeeded
        bool init();

        [[nodiscard]]
        bool isLoaded() const;

        [[nodiscard]]
        Shader *shader();

        [[nodiscard]]
        const Shader *shader() const;

        RenderProgram &setIndices(const uint *indices, int count, bool dynamic = false);
        RenderProgram &setIndices(const vector<uint> &indices, const bool dynamic = false)
        {
            return setIndices(indices.data(), static_cast<int>(indices.size()), dynamic);
        }

        RenderProgram &setVertices(const void *vertices, int count, bool dynamic = false);

        template <typename T>
        RenderProgram &setVertices(const vector<T> &vertices, bool dynamic = false)
        {
            if (typeid(T).hash_code() != m_config.attributes.vertexTypeHash())
            {
                SDGL_ERROR("Vertex type mismatch");
                return *this;
            }

            return setVertices(vertices.data(), vertices.size(), dynamic);
        }

        void clearIndices();

        /// Draw the program
        void render(PrimitiveType::Enum primitiveType = PrimitiveType::Triangles) const;

        /// Clean up, call init again to reinitialize
        void dispose();

    private:
        uint m_vao, m_vbo, m_ebo;
        Shader m_shader;
        Config m_config;

        int m_indexCount, m_vertexCount;
    };
}
