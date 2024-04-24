#pragma once
#include <sdgl/sdglib.h>

#include "Asset.h"
#include "graphics/Texture2D.h"

namespace sdgl {
    // Forward declarations
    class BitmapFont;
    class TextureAtlas;


    class ContentManager
    {
    public:
        ContentManager() : m_cache() { }
        ~ContentManager();

        Texture2D *loadTexture(const string &filepath, TextureFilter::Enum flags = TextureFilter::Nearest);

        BitmapFont *loadBitmapFont(const string &filepath);

        TextureAtlas *loadTextureAtlas(const string &filepath);

        /// Unload an asset that was previously loaded at a filepath
        /// @param filepath path that the asset was previously loaded from
        /// @returns whether unload succeeded
        bool unload(const string &filepath);

        void unloadAll();

    private:
        template<typename T>
        T *checkCache(const string &filepath)
        {
            if (const auto it = m_cache.find(filepath); it != m_cache.end())
                return dynamic_cast<T *>(it->second);
            return nullptr;
        }

        map<string, Asset *> m_cache;
    };
}

