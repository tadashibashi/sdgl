#pragma once
#include <sdgl/graphics/Texture2D.h>

namespace sdgl {
    struct CrunchAtlasData
    {
        /// Load crunch atlas data from a binary file
        /// @param buffer        crunch binary file data
        /// @param trimEnabled   whether image trimming was enabled on exported atlas
        /// @param rotateEnabled whether image rotation was enabled on exported atlas
        /// @param outData [out] pointer to receive the data - must not be null
        ///
        /// @returns whether function was successful
        static bool loadBinary(const string &buffer, bool trimEnabled, bool rotateEnabled, CrunchAtlasData *outData);

        struct Image
        {
            string name;
            int16 x;
            int16 y;
            int16 width;
            int16 height;
            int16 frameX;
            int16 frameY;
            int16 frameWidth;
            int16 frameHeight;
            ubyte rotated;
        };

        /// Atlas texture, contains a set of subimages (struct Image)
        struct Texture
        {
            string name;          ///< name of the associated image file
            vector<Image> images; ///< list of sub-image data for this atlas texture
        };

        vector<Texture> textures;
    };

}

