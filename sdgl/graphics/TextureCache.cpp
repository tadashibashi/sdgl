#include "TextureCache.h"

#include <stdexcept>

namespace sdgl {
    TextureCache::TextureCache() : m_textures()
    {
    }

    TextureCache::~TextureCache()
    {
        unload();
    }

    const Texture2D &TextureCache::load(const string &path)
    {
        if (const auto it = m_textures.find(path);
            it != m_textures.end())
        {
            return it->second;
        }

        if (auto newTexture = Texture2D();
            newTexture.loadFile(path))
        {
            m_textures[path] = newTexture;
            return m_textures[path];
        }

        throw std::runtime_error("Failed to load texture file");
    }

    bool TextureCache::unload(const string &path)
    {
        if (const auto it = m_textures.find(path);
            it != m_textures.end())
        {
            it->second.free();
            m_textures.erase(it);
            return true;
        }

        return false;
    }

    void TextureCache::unload()
    {
        for (auto &[key, value] : m_textures)
        {
            value.free();
        }

        m_textures.clear();
    }
}
