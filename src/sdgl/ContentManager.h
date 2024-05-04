#pragma once
#include <sdgl/sdglib.h>

#include "Asset.h"
#include "graphics/Texture2D.h"

namespace sdgl {
    // Forward declarations
    class BitmapFont;
    class TextureAtlas;

    /// Manages loading textures, fonts, etc. Caches each asset per filepath
    class ContentManager
    {
    public:
        ContentManager() : m_cache() { }
        ~ContentManager();

        /// Load a 2D texture from a .png, .jpg, .tga, .bmp, or .hdr file.
        /// Make sure to set desired filter type via Texture2D
        /// @param filepath path to the image file
        /// @param flags    filter type
        /// @return loaded or cached texture
        const Texture2D *loadTexture(const fs::path &filepath);

        /// Load a bitmap font
        /// @param filepath
        /// @return
        const BitmapFont *loadBitmapFont(const fs::path &filepath);

        const TextureAtlas *loadTextureAtlas(const fs::path &filepath);

        /// Unload an asset that was previously loaded
        /// @param filepath path that the asset was previously loaded from
        /// @returns whether unload succeeded - it will not if filepath doesn't exist in cache
        bool unload(const fs::path &filepath);

        void unloadAll();

    private:
        template<typename T>
        T *checkCache(const fs::path &filepath)
        {
            if (const auto it = m_cache.find(filepath.native()); it != m_cache.end())
                return dynamic_cast<T *>(it->second);
            return nullptr;
        }

        map<fs::path::string_type, Asset *> m_cache;
    };
}

