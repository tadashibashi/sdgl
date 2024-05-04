#include "ContentManager.h"

#include "graphics/atlas/TextureAtlas.h"
#include "graphics/font/BitmapFont.h"

namespace sdgl {
    ContentManager::~ContentManager()
    {
        // clean up any remaining assets
        unloadAll();
    }

    const Texture2D *ContentManager::loadTexture(const fs::path &filepath)
    {
        if (const auto cached = checkCache<Texture2D>(filepath))
            return cached;
        auto texture = new Texture2D();
        if (!texture->loadFile(filepath))
        {
            delete texture;
            return nullptr;
        }

        m_cache[filepath] = texture;
        return texture;
    }

    const BitmapFont *ContentManager::loadBitmapFont(const fs::path &filepath)
    {
        if (const auto cached = checkCache<BitmapFont>(filepath))
            return cached;
        auto font = new BitmapFont();
        if (!font->loadBMFont(filepath))
        {
            delete font;
            return nullptr;
        }

        m_cache[filepath] = font;
        return font;
    }

    const TextureAtlas *ContentManager::loadTextureAtlas(const fs::path &filepath)
    {
        if (const auto cached = checkCache<TextureAtlas>(filepath))
            return cached;
        auto atlas = new TextureAtlas();
        if (!atlas->loadCrunch(filepath))
        {
            delete atlas;
            return nullptr;
        }

        m_cache[filepath] = atlas;
        return atlas;
    }

    bool ContentManager::unload(const fs::path &filepath)
    {
        // See if this container holds this asset in memory
        auto it = m_cache.find(filepath.native());
        if (it == m_cache.end())
        {
            // It doesn't
            return false;
        }

        // It does, unload and delete it
        it->second->unload();
        delete it->second;
        m_cache.erase(it);

        return true;
    }

    void ContentManager::unloadAll()
    {
        for (auto &[key, asset] : m_cache)
        {
            asset->unload();
            delete asset;
        }

        m_cache.clear();
    }
}
