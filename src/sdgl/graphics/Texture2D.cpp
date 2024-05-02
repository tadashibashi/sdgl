#include "Texture2D.h"

#include <sdgl/angles.h>
#include <sdgl/io/io.h>

#include <stb_image.h>

namespace sdgl {
    bool Texture2D::loadFile(const string &filepath, const TextureFilter::Enum filter)
    {
        string buffer;
        if (!io::readFile(filepath, &buffer))
            return false;

        return loadMem(buffer, filter);
    }

    bool Texture2D::loadMem(const string &buffer, const TextureFilter::Enum filter)
    {
        //stbi_set_flip_vertically_on_load(true);

        int width, height, bytesPerPixel;
        const auto data = stbi_load_from_memory(
            reinterpret_cast<const stbi_uc *>(buffer.data()),
            static_cast<int>(buffer.size()),
            &width, &height, &bytesPerPixel, 0);

        if (!data)
        {
            SDGL_ERROR("stb_image failed to load image: {}", stbi_failure_reason());
            return false;
        }

        const auto result = loadBytes(data, width * height * bytesPerPixel, width, height, filter);
        stbi_image_free(data);

        return result;
    }

    bool Texture2D::loadBytes(const void *data, size_t length, int width, int height, TextureFilter::Enum filter)
    {
        SDGL_ASSERT(width * height * 4 == length, "Ensure texture dimensions match buffer size");

        // Generate the GL texture object
        GLuint textureId;
        glGenTextures(1, &textureId);
        if (glGetError() != GL_NO_ERROR)
        {
            SDGL_ERROR("Failed to load Texture2D from bytes: GL failed to generate texture. Error code: {}",
                glGetError());
            return false;
        }

        try
        {
            // Pass image data to the texture object
            glBindTexture(GL_TEXTURE_2D, textureId); GL_ERR_CHECK();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                static_cast<GLsizei>(width),
                static_cast<GLsizei>(height), 0, GL_RGBA,
                GL_UNSIGNED_BYTE, data); GL_ERR_CHECK();

            // Set texture parameters
            int texFilter = filter == TextureFilter::Bilinear ? GL_LINEAR : GL_NEAREST;

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); GL_ERR_CHECK();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); GL_ERR_CHECK();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texFilter); GL_ERR_CHECK();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texFilter); GL_ERR_CHECK();
            glGenerateMipmap(GL_TEXTURE_2D); GL_ERR_CHECK();

            // Done, unbind texture
            glBindTexture(GL_TEXTURE_2D, 0); GL_ERR_CHECK(); // unbind

            // Clean up pre-existing texture, if any
            if (m_id)
            {
                glDeleteTextures(1, &m_id); GL_ERR_CHECK();
            }

            // Commit data
            m_id = textureId;
            m_size = {width, height};
            return true;
        }
        catch(const std::exception &_)
        {
            // GL_ERR_CHECK handles error reporting
            glDeleteTextures(1, &textureId);
            return false;
        }
        catch(...)
        {
            SDGL_ERROR("Failed to load Texture2D from pixel data: unknown error");
            glDeleteTextures(1, &textureId);
            return false;
        }
    }

    bool Texture2D::loadBytes(const string_view buffer, const int width, const int height, const TextureFilter::Enum filter)
    {
        return loadBytes(buffer.data(), buffer.size(), width, height, filter);
    }

    bool Texture2D::loadBytes(const vector<Color> &pixels, int width, int height, TextureFilter::Enum filter)
    {
        SDGL_ASSERT(!pixels.empty());

        return loadBytes(&pixels[0].r, pixels.size() * sizeof(Color), width, height, filter);
    }

    void Texture2D::unload()
    {
        if (m_id)
        {
            glDeleteTextures(1, &m_id);
            m_id = 0;
            m_size = {};
        }
    }
}
