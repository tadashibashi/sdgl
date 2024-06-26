#include "RenderProgram.h"

#include <sdgl/angles.h>
#include <utility>

namespace sdgl {
    RenderProgram::RenderProgram(Config config) :
      m_vao(0), m_vbo(0), m_ebo(0), m_shader(), m_config(std::move(config)), m_indexCount(0), m_vertexCount(0)
    {
    }

    RenderProgram::RenderProgram() :
      m_vao(0), m_vbo(0), m_ebo(0), m_shader(), m_config(), m_indexCount(0), m_vertexCount(0)
    {
    }

    RenderProgram::RenderProgram(RenderProgram &&other) noexcept :
      m_vao(other.m_vao), m_vbo(other.m_vbo), m_ebo(other.m_ebo),
      m_shader(std::move(other.m_shader)),
      m_config(std::move(other.m_config)),
      m_indexCount(other.m_indexCount), m_vertexCount(other.m_vertexCount)
    {
        // Invalidate other
        other.m_vertexCount = 0;
        other.m_indexCount = 0;
        other.m_vao = 0;
        other.m_vbo = 0;
        other.m_ebo = 0;
    }

    RenderProgram &RenderProgram::operator=(RenderProgram &&other) noexcept
    {
        m_vao = other.m_vao;
        m_vbo = other.m_vbo;
        m_ebo = other.m_ebo;
        m_shader = std::move(other.m_shader);
        m_config = std::move(other.m_config);
        m_indexCount = other.m_indexCount;
        m_vertexCount = other.m_vertexCount;

        // Invalidate other
        other.m_vertexCount = 0;
        other.m_indexCount = 0;
        other.m_vao = 0;
        other.m_vbo = 0;
        other.m_ebo = 0;

        return *this;
    }

    RenderProgram::~RenderProgram()
    {
        dispose();
    }

    bool RenderProgram::init()
    {
        SDGL_ASSERT(!m_config.vertShader.empty()); // must have vertex shader data
        SDGL_ASSERT(!m_config.fragShader.empty()); // must have fragment shader data
        SDGL_ASSERT(!m_config.attributes.empty()); // must be at least one vertex type in there

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

    bool RenderProgram::init(Config config)
    {
        m_config = std::move(config);
        return init();
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
        m_indexCount = 0;
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

    void RenderProgram::render(PrimitiveType::Enum primitiveType, int offset, int count) const
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
            // constrain indices for best performance of glDrawRangeElements
            offset = mathf::clampi(offset, 0, m_indexCount);
            auto maxIndex =  mathf::clampi(offset + count - 1, offset, m_indexCount);
            count = mathf::clampi(count, 0, m_indexCount - offset + 1);

            glDrawRangeElements(glPrimitiveType, offset, maxIndex, count, GL_UNSIGNED_INT, (void *)(sizeof(uint) * offset) ) ; GL_ERR_CHECK();
            glBindVertexArray(0); GL_ERR_CHECK();
        }
        else                  // render vertices directly
        {
            offset = mathf::clampi(offset, 0, m_vertexCount);
            count = mathf::clampi(count, 0, m_vertexCount - offset + 1);

            glDrawArrays(glPrimitiveType, offset, count); GL_ERR_CHECK();
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

        m_shader.dispose();
    }
}
