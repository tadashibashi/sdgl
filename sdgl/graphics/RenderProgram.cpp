#include "RenderProgram.h"

#include <sdgl/gl.h>

#include <utility>

namespace sdgl::graphics {
    RenderProgram::RenderProgram(Config config) :
        m_vao(0), m_vbo(0), m_ebo(0), m_config(std::move(config)), m_indexCount(0), m_vertexCount(0)
    {
    }

    RenderProgram::RenderProgram() :
        m_vao(0), m_vbo(0), m_ebo(0), m_config(), m_indexCount(0), m_vertexCount(0)
    {
    }

    bool RenderProgram::init()
    {
        // Load shader
        Shader shader;
        if (m_config.openFiles)
        {
            shader.loadFiles(m_config.vertShader, m_config.fragShader);
        }
        else
        {
            shader.load(m_config.vertShader, m_config.fragShader);
        }

        if (!shader.isLoaded())
        {
            return false;
        }

        // Setup buffer objects
        GLuint vao{0}, vbo{0}, ebo{0};

        glGenVertexArrays(1, &vao); GL_ERR_CHECK();
        glGenBuffers(1, &vbo); GL_ERR_CHECK();
        glGenBuffers(1, &ebo); GL_ERR_CHECK();

        SDGL_ASSERT(vao);
        SDGL_ASSERT(vbo);
        SDGL_ASSERT(ebo);

        // Set up the vertex array object
        glBindVertexArray(vao); GL_ERR_CHECK();
        glBindBuffer(GL_ARRAY_BUFFER, vbo); GL_ERR_CHECK();


        for (uint i = 0; const auto &[count, normalized, type, stride, offset] : m_config.attributes)
        {
            glEnableVertexAttribArray(i); GL_ERR_CHECK();
            glVertexAttribPointer(i, count, type, normalized, static_cast<int>(stride),
              (void *)(uintptr_t)offset); GL_ERR_CHECK();
            ++i;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); GL_ERR_CHECK();

        // Unbind buffers
        glBindVertexArray(0); GL_ERR_CHECK();
        glBindBuffer(GL_ARRAY_BUFFER, 0); GL_ERR_CHECK();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GL_ERR_CHECK();

        // Commit changes
        if (isLoaded())
        {
            dispose();
        }

        m_shader = std::move(shader);
        m_vao = vao;
        m_vbo = vbo;
        m_ebo = ebo;

        return true;
    }

    bool RenderProgram::isLoaded() const
    {
        return m_shader.isLoaded() && m_vao && m_vbo && m_ebo;
    }

    Shader *RenderProgram::shader()
    {
        return &m_shader;
    }

    const Shader *RenderProgram::shader() const
    {
        return &m_shader;
    }

    RenderProgram &RenderProgram::setIndices(const uint *indices, int count, bool dynamic)
    {
        SDGL_ASSERT(m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo); GL_ERR_CHECK();

        const auto size = static_cast<GLsizeiptr>(sizeof(uint) * count);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr,
            dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW); GL_ERR_CHECK();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, indices); GL_ERR_CHECK();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GL_ERR_CHECK();

        m_indexCount = count;
        return *this;
    }

    RenderProgram &RenderProgram::setVertices(const void *vertices, int count, bool dynamic)
    {
        SDGL_ASSERT(m_vbo);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo); GL_ERR_CHECK();

        const auto size = static_cast<GLsizeiptr>(m_config.attributes.sizeofVertex() * count);

        // orphan buffer, then fill it
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW); GL_ERR_CHECK();
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices); GL_ERR_CHECK();

        glBindBuffer(GL_ARRAY_BUFFER, 0); GL_ERR_CHECK();

        m_vertexCount = count;
        return *this;
    }

    void RenderProgram::clearIndices()
    {
        if (m_ebo && m_indexCount)
        {
            m_indexCount = 0;
        }
    }

    static int s_primitiveTypes[] = {
        GL_POINTS,
        GL_LINES,
        GL_LINE_STRIP,
        GL_LINE_LOOP,
        GL_TRIANGLES,
        GL_TRIANGLE_STRIP,
        GL_TRIANGLE_FAN,
    };

    void RenderProgram::render(PrimitiveType::Enum primitiveType) const
    {
        SDGL_ASSERT(m_shader.isLoaded());
        SDGL_ASSERT(m_vao);
        SDGL_ASSERT(m_vbo);
        SDGL_ASSERT(m_ebo);

        m_shader.use();

        glBindVertexArray(m_vao); GL_ERR_CHECK();

        auto glPrimitiveType = s_primitiveTypes[primitiveType];
        if (m_indexCount > 0) // render by index
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo); GL_ERR_CHECK();
            glDrawElements(glPrimitiveType, m_indexCount, GL_UNSIGNED_INT, nullptr); GL_ERR_CHECK();
            glBindVertexArray(0); GL_ERR_CHECK();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GL_ERR_CHECK();
        }
        else                  // render vertices directly
        {
            glDrawArrays(glPrimitiveType, 0, m_vertexCount); GL_ERR_CHECK();
            glBindVertexArray(0); GL_ERR_CHECK();
        }

        m_shader.unuse();
    }

    void RenderProgram::dispose()
    {
        if (m_ebo)
        {
            glDeleteBuffers(1, &m_ebo);
            m_ebo = 0;
        }

        if (m_vbo)
        {
            glDeleteBuffers(1, &m_vbo);
            m_vbo = 0;
        }

        if (m_vao)
        {
            glDeleteVertexArrays(1, &m_vao);
            m_vao = 0;
        }

        m_shader.clear();
    }
}
