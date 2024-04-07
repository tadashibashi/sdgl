#pragma once
#include <sdgl/graphics/Texture2D.h>

namespace sdgl {
    class TextureCache {
    public:
        TextureCache();
        ~TextureCache();

        /// Load a texture or grab it from the cache, if already loaded
        const Texture2D &load(const string &path);

        const Texture2D &operator[](const string &path)
        {
            return load(path);
        }

        TextureCache &operator -=(const string &path)
        {
            unload(path);
            return *this;
        }

        /// Unload a texture. Prefer to unload via this function istead of directly on the Texture2D.
        /// @param path - texture at path to unload
        /// @returns whether file was unloaded
        bool unload(const string &path);

        /// Unload entire cache
        void unload();


    private:
        map<string, Texture2D> m_textures;
    };
}
