#include "Shader.h"
#include <sdgl/gl.h>
#include <sdgl/logging.h>
#include <sdgl/io/io.h>

namespace sdgl {
    Shader::Shader() : m_program(), m_attribCounter() { }

    Shader::Shader(Shader &&other) noexcept : m_program(other.m_program), m_attribCounter(other.m_attribCounter)
    {

    }

    Shader::~Shader()
    {
        if (m_program)
        {
            glDeleteProgram(m_program);
        }
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

    int Shader::getLocation(const std::string &uniformName) const
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

    void Shader::addAttribute(const string &name)
    {
        if (!isLoaded())
        {
            SDGL_ERROR("Attempted to add a shader attribute '{}', but shader was not loaded.", name);
            return;
        }

        glBindAttribLocation(m_program, m_attribCounter++, name.c_str());
    }

    void Shader::use() const
    {
        glUseProgram(m_program);
    }

    void Shader::clear()
    {
        if (m_program)
        {
            glDeleteProgram(m_program);
            m_program = 0;
            m_attribCounter = 0;
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

