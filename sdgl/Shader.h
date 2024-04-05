#pragma once
#include <glm/fwd.hpp>

#include "sdglib.h"
#include "ShaderAttribs.h"
#include "Texture2D.h"

namespace sdgl {
    /**
     * OpenGL primitives to draw
     */
     struct DrawType
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

    /**
     * RAII container that manages a shader program.
     */
    class Shader {
    public:
        Shader();
        Shader(Shader &&other) noexcept;
        ~Shader();

        Shader &operator=(Shader &&other) noexcept;

        /**
         * Load shader program from files
         * @param vsPath - path to vertex shader file
         * @param fsPath - path to fragment shader file
         * @return whether compilation and linkage succeeded
         */
        bool loadFiles(const string &vsPath, const string &fsPath);

        /**
         * Load shader program from strings
         * @param vsString - vertex shader string
         * @param fsString - fragment shader string
         * @return whether compilation and linkage succeeded
         */
        bool load(const string &vsString, const string &fsString);

        /**
         * @return whether shader program is linked and loaded
         */
        [[nodiscard]]
        bool isLoaded() const { return static_cast<bool>(m_program); }

        /**
         * Get the location of a uniform in the shader to cache for efficiency
         * @param uniformName
         * @return
         */
        [[nodiscard]]
        int locateUniform(const std::string &uniformName) const;

        Shader &setUniform(int location, const float *value, int count);
        Shader &setUniform(int location, const glm::mat4 *value, int count, bool transpose = false);
        Shader &setUniform(int location, const glm::vec4 *value, int count);
        Shader &setUniform(int location, const glm::vec3 *value, int count);
        Shader &setUniform(int location, const glm::vec2 *value, int count);
        Shader &setUniform(int location, const int *value, int count);
        Shader &setUniform(int location, const uint *value, int count);
        Shader &setUniform(int location, const Texture2D *textures, int count, int slot = 0);

        Shader &setUniform(int location, float value);
        Shader &setUniform(int location, const glm::mat4 &value, bool transpose = false);
        Shader &setUniform(int location, const glm::vec4 &value);
        Shader &setUniform(int location, const glm::vec3 &value);
        Shader &setUniform(int location, const glm::vec2 &value);
        Shader &setUniform(int location, const Vector2 &value);
        Shader &setUniform(int location, int value);
        Shader &setUniform(int location, uint value);
        Shader &setUniform(int location, const Texture2D &texture, int slot = 0);

        /**
         * 4x4 matrix
         * @param location
         * @param value
         * @param transpose
         * @return
         */
        Shader &setUniformMatrix(int location, const float *value, bool transpose = false);

        template <typename T> requires std::is_pointer_v<T>
        Shader &setUniform(const string &name, T value, int count)
        {
            return setUniform(locateUniform(name), value, count);
        }

        template <typename T> requires (!std::is_pointer_v<T>)
        Shader &setUniform(const string &name, T value)
        {
            return setUniform(locateUniform(name), value);
        }

        void use() const;

        void unuse() const;

        /**
         * Delete loaded shader program
         */
        void dispose();
    private:
        static uint compileFile(const string &filepath, int type);
        static uint compileString(const char *source, int type);
        static bool linkProgram(uint program, uint vs, uint fs, bool deleteShaders = true);

        uint m_program;

    };
}
