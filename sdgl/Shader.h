#pragma once
#include "sdglib.h"

namespace sdgl {
    /**
     * RAII container that manages a shader program.
     */
    class Shader {
    public:
        Shader();
        Shader(Shader &&other) noexcept;
        Shader(const Shader &other) = delete;
        ~Shader();

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
         * Get the location of a uniform in the shader
         * @param uniformName
         * @return
         */
        [[nodiscard]]
        int getLocation(const std::string &uniformName) const;

        void addAttribute(const string &name);

        void use() const;

        /**
         * Delete loaded shader program
         */
        void clear();
    private:
        static uint compileFile(const string &filepath, int type);
        static uint compileString(const char *source, int type);
        static bool linkProgram(uint program, uint vs, uint fs, bool deleteShaders = true);

        uint m_program;
        uint m_attribCounter;
    };
}
