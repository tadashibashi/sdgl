#include "Shader.h"
#include <sdgl/gl.h>
#include <sdgl/logging.h>
#include <sdgl/io/io.h>
#include <glm/glm.hpp>

namespace sdgl {
    Shader::Shader() : m_program() { }

    Shader::Shader(Shader &&other) noexcept : m_program(other.m_program)
    {
        other.m_program = 0;
    }

    Shader::~Shader()
    {
        if (m_program)
        {
            glDeleteProgram(m_program);
        }
    }

    Shader &Shader::operator=(Shader &&other) noexcept
    {
        if (m_program)
        {
            glDeleteProgram(m_program);
        }

        m_program = other.m_program;
        other.m_program = 0;

        return *this;
    }

    bool Shader::loadFiles(const string &vsPath, const string &fsPath)
    {
        const auto vs = compileFile(vsPath, GL_VERTEX_SHADER);
        if (!vs)
            return false;
        const auto fs = compileFile(fsPath, GL_FRAGMENT_SHADER);
        if (!fs)
            return false;

        const auto program = glCreateProgram();

        if (!linkProgram(program, vs, fs, true))
        {
            glDeleteProgram(program);
            return false;
        }

        // clean up pre-existing shader
        clear();

        m_program = program;
        return true;
    }


    bool Shader::load(const string &vsString, const string &fsString)
    {
        const auto vs = compileString(vsString.c_str(), GL_VERTEX_SHADER);
        if (!vs)
            return false;
        const auto fs = compileString(fsString.c_str(), GL_FRAGMENT_SHADER);
        if (!fs)
            return false;

        const auto program = glCreateProgram();

        if (!linkProgram(program, vs, fs, true))
        {
            glDeleteProgram(program);
            return false;
        }

        // clean up pre-existing shader
        clear();

        m_program = program;
        return true;
    }

    int Shader::locateUniform(const std::string &uniformName) const
    {
        if (!isLoaded())
        {
            SDGL_ERROR("Could not get the location of uniform '{}': Shader program is not loaded", uniformName);
            return -1;
        }

        auto location = glGetUniformLocation(m_program, uniformName.c_str());
        if (location == GL_INVALID_VALUE)
        {
            SDGL_ERROR("Could not find the location of uniform '{}' in shader program", uniformName);
            return -1;
        }

        return location;
    }

    Shader &Shader::setUniform(int location, const float *value, int count)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glUniform1fv(location, count, value);
        GL_ERR_CHECK();
        return *this;
    }

    Shader &Shader::setUniform(int location, const glm::mat4 *value, int count, bool transpose)
    {
        glUseProgram(m_program);
        glUniformMatrix4fv(location, count, transpose, &(*value)[0][0]);
        GL_ERR_CHECK();
        return *this;
    }

    Shader &Shader::setUniform(int location, const glm::vec4 *value, int count)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glUniform4fv(location, count, &value->x);
        GL_ERR_CHECK();
        return *this;
    }

    Shader &Shader::setUniform(int location, const glm::vec3 *value, int count)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glUniform3fv(location, count, &value->x);
        GL_ERR_CHECK();
        return *this;
    }

    Shader &Shader::setUniform(int location, const glm::vec2 *value, int count)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glUniform2fv(location, count, &value->x);
        GL_ERR_CHECK();
        return *this;
    }

    Shader &Shader::setUniform(int location, const int *value, int count)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glUniform1iv(location, count, value);
        GL_ERR_CHECK();
        return *this;
    }

    Shader &Shader::setUniform(int location, const uint *value, int count)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glUniform1uiv(location, count, value);
        GL_ERR_CHECK();
        return *this;
    }

    Shader &Shader::setUniform(int location, const Texture2D *textures, int count, int slot)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        for (auto i = 0; i < count; ++i)
        {
            setUniform(location, textures[i], slot + i);
            GL_ERR_CHECK();
        }

        return *this;
    }

    Shader &Shader::setUniform(const int location, const float value)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glUniform1f(location, value);
        GL_ERR_CHECK();
        return *this;
    }

    Shader &Shader::setUniform(int location, const glm::mat4 &value, bool transpose)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glUniformMatrix4fv(location, 1, transpose, &value[0][0]);
        GL_ERR_CHECK();
        return *this;
    }

    Shader &Shader::setUniform(int location, const glm::vec4 &value)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glUniform4f(location, value.x, value.y, value.z, value.w);
        GL_ERR_CHECK();
        return *this;
    }

    Shader &Shader::setUniform(int location, const glm::vec3 &value)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glUniform3f(location, value.x, value.y, value.z);
        GL_ERR_CHECK();
        return *this;
    }

    Shader &Shader::setUniform(int location, const glm::vec2 &value)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glUniform2f(location, value.x, value.y);
        GL_ERR_CHECK();
        return *this;
    }

    Shader &Shader::setUniform(const int location, const int value)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glUniform1i(location, value);
        GL_ERR_CHECK();
        return *this;
    }

    Shader &Shader::setUniform(const int location, const uint value)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glUniform1ui(location, value);
        GL_ERR_CHECK();
        return *this;
    }

    static int s_textureSlots[32] = {
        GL_TEXTURE0,
        GL_TEXTURE1,
        GL_TEXTURE2,
        GL_TEXTURE3,
        GL_TEXTURE4,
        GL_TEXTURE5,
        GL_TEXTURE6,
        GL_TEXTURE7,
        GL_TEXTURE8,
        GL_TEXTURE9,
        GL_TEXTURE10,
        GL_TEXTURE11,
        GL_TEXTURE12,
        GL_TEXTURE13,
        GL_TEXTURE14,
        GL_TEXTURE15,
        GL_TEXTURE16,
        GL_TEXTURE17,
        GL_TEXTURE18,
        GL_TEXTURE19,
        GL_TEXTURE20,
        GL_TEXTURE21,
        GL_TEXTURE22,
        GL_TEXTURE23,
        GL_TEXTURE24,
        GL_TEXTURE25,
        GL_TEXTURE26,
        GL_TEXTURE27,
        GL_TEXTURE28,
        GL_TEXTURE29,
        GL_TEXTURE30,
        GL_TEXTURE31,
    };

    Shader &Shader::setUniform(int location, const Texture2D &texture, int slot)
    {
        SDGL_ASSERT(slot >= 0 && slot < 32);
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glActiveTexture(s_textureSlots[slot]);
        GL_ERR_CHECK();
        glBindTexture(GL_TEXTURE_2D, texture.id());
        GL_ERR_CHECK();
        glUniform1i(location, slot);
        GL_ERR_CHECK();
        return *this;
    }

    Shader &Shader::setUniformMatrix(int location, const float *value, bool transpose)
    {
        glUseProgram(m_program);
        GL_ERR_CHECK();
        glUniformMatrix4fv(location, 1, transpose, value);
        GL_ERR_CHECK();
        return *this;
    }

    void Shader::use() const
    {
        SDGL_ASSERT(m_program);
        glUseProgram(m_program); GL_ERR_CHECK();
    }

    void Shader::unuse() const
    {
        glUseProgram(0); GL_ERR_CHECK();
    }

    void Shader::clear()
    {
        if (m_program)
        {
            glDeleteProgram(m_program);
            m_program = 0;
        }
    }

    uint Shader::compileFile(const string &filepath, const int type)
    {
        string data;
        if (io::readFile(filepath, &data))
        {
            return 0;
        }

        return compileString(data.c_str(), type);
    }

    uint Shader::compileString(const char *source, const int type)
    {
        const auto shader = glCreateShader(type);
        if (!shader)
        {
            SDGL_ERROR("Shader failed to create: {}", glGetError());
            return 0;
        }

        // load and compile shader string
        glShaderSource(shader, 1, &source, nullptr); // null stops at first '\0' instead of traversing array of strings
        glCompileShader(shader);

        // check for compile errors
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE)
        {
            char result[128];
            glGetShaderInfoLog(shader, 128, nullptr, result);

            SDGL_ERROR("Shader failed to compile: {}", result);

            glDeleteShader(shader);
            return 0;
        }

        // success!
        return shader;
    }

    bool Shader::linkProgram(uint program, uint vs, uint fs, bool deleteShaders)
    {
        // Link shaders to program program
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLchar buffer[256];
            glGetProgramInfoLog(program, 256, nullptr, buffer);

            SDGL_ERROR("Shader program failed to link: {}", buffer);

            if (deleteShaders)
            {
                glDeleteShader(vs);
                glDeleteShader(fs);
            }

            return false;
        }

        if (deleteShaders)
        {
            glDetachShader(program, vs);
            glDetachShader(program, fs);
            glDeleteShader(vs);
            glDeleteShader(fs);
        }

        return true;
    }
}

