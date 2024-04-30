#include "../ShaderAttribs.h"
#include "gl.h"

namespace sdgl {
    struct GLTypeHash
    {
        static const size_t Byte;
        static const size_t Ubyte;
        static const size_t Short;
        static const size_t Ushort;
        static const size_t Int;
        static const size_t Uint;
        static const size_t Float;
    };
    const size_t GLTypeHash::Byte = typeid(byte).hash_code();
    const size_t GLTypeHash::Ubyte = typeid(ubyte).hash_code();
    const size_t GLTypeHash::Short = typeid(short).hash_code();
    const size_t GLTypeHash::Ushort = typeid(ushort).hash_code();
    const size_t GLTypeHash::Int = typeid(int).hash_code();
    const size_t GLTypeHash::Uint = typeid(uint).hash_code();
    const size_t GLTypeHash::Float = typeid(float).hash_code();

    static int s_enumToGLEnum[] = {
        GL_BYTE,
        GL_UNSIGNED_BYTE,
        GL_SHORT,
        GL_UNSIGNED_SHORT,
        GL_INT,
        GL_UNSIGNED_INT,
        GL_FLOAT,
    };

    ShaderAttribs &ShaderAttribs::attrib(size_t hash, int count, bool normalized, uint stride, uint offset)
    {
        // Get gl type enum value
        auto typeEnum =
            hash == GLTypeHash::Float ? GL_FLOAT :
            hash == GLTypeHash::Byte ? GL_BYTE :
            hash == GLTypeHash::Ubyte ? GL_UNSIGNED_BYTE :
            hash == GLTypeHash::Int ? GL_INT :
            hash == GLTypeHash::Uint ? GL_UNSIGNED_INT :
            hash == GLTypeHash::Short ? GL_SHORT :
            hash == GLTypeHash::Ushort ? GL_UNSIGNED_SHORT :
            hash ==
            GL_NONE;

        SDGL_ASSERT(typeEnum != GL_NONE); // type msut be supported

        m_attribs.emplace_back(
          Attribute{
            .count = count,
            .normalized = normalized,
            .type = typeEnum,
            .stride = stride,
            .offset = offset,
        });

        return *this;
    }

    ShaderAttribs &ShaderAttribs::attrib(GLType::Enum type, int count, bool normalized, uint stride, uint offset)
    {
        const auto typeEnum = s_enumToGLEnum[type];

        m_attribs.emplace_back(
          Attribute {
            .count = count,
            .normalized = normalized,
            .type = typeEnum,
            .stride = stride,
            .offset = offset,
        });

        return *this;
    }
}
