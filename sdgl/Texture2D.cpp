#include "Texture2D.h"

#include "io/io.h"
#include <stb_image.h>
#include <sdgl/gl.h>

namespace sdgl {
    bool Texture2D::loadFile(const string &filepath, TextureFilter::Enum filter)
    {
        string buffer;
        if (!io::readFile(filepath, &buffer))
            return false;

        return load(buffer);
    }

    bool Texture2D::load(const string &buffer, TextureFilter::Enum filter)
    {
        int width, height, bytesPerPixel;
        auto data = stbi_load_from_memory(
            reinterpret_cast<const stbi_uc *>(buffer.data()),
            static_cast<int>(buffer.size()),
            &width, &height, &bytesPerPixel, 0);

        if (!data)
        {
            SDGL_ERROR("Stb Image failed to load image: {}", stbi_failure_reason());
            return false;
        }

        try
        {
            GLuint textureId;
            glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);

            // pass texture the image data
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                static_cast<GLsizei>(width),
                static_cast<GLsizei>(height), 0, GL_RGBA,
                GL_UNSIGNED_BYTE, data);

            int texFilter = filter == TextureFilter::Bilinear ? GL_LINEAR : GL_NEAREST;

            // texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texFilter);
            glGenerateMipmap(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, 0); // unbind

            if (m_id)
            {
                glDeleteTextures(1, &m_id);
            }

            m_id = textureId;
            m_size = {width, height};

            stbi_image_free(data);
            return true;
        }
        catch(...)
        {
            stbi_image_free(data);
            return false;
        }

    }

    void Texture2D::free()
    {
        if (m_id)
        {
            glDeleteTextures(1, &m_id);
            m_id = 0;
            m_size = {};
        }
    }
}
