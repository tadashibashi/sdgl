#pragma once
#include <sdgl/graphics/Shader.h>
#include <sdgl/graphics/ShaderAttribs.h>

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
        RenderProgram(RenderProgram &&other) noexcept;
        RenderProgram &operator=(RenderProgram &&other) noexcept;
        RenderProgram(const RenderProgram &other) = delete;

        ~RenderProgram();

        /// Initialize the program with the currently loaded config, this must be called before `render`
        /// @returns whether initialization succeeded
        bool init();

        /// Initialize the program with a new config - this must be called before `render`
        /// @param config configuration object, if one was already passed in the ctor,
        ///               just call `init()`
        /// @returns whether initialization succeeded
        bool init(Config config);

        /// Check if the program has been initialized successfully
        [[nodiscard]]
        bool isLoaded() const;

        /// Get the shader - use this reference to set/get uniforms
        [[nodiscard]]
        Shader *shader();

        /// Get the shader - use this reference to get uniforms
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
        /// @param primitiveType type of primitives to render vertices with - corresponds to OpenGL mode
        /// @param offset number of vertices or indices (if setIndices was called) to begin drawing from
        /// @param count number of vertices or indices (if setIndices was called) to draw, by default all are drawn
        void render(PrimitiveType::Enum primitiveType = PrimitiveType::Triangles, int offset = 0, int count = INT_MAX) const;

        /// Clean up - all calls to render after calling `dispose` will fail until `init` is called again
        void dispose();

    private:
        uint m_vao, m_vbo, m_ebo;
        Shader m_shader;
        Config m_config;

        int m_indexCount, m_vertexCount;
    };
}
