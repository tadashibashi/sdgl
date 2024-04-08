#pragma once
#include <sdgl/graphics/Texture2D.h>

namespace sdgl::graphics {
    struct CrunchAtlasData
    {
        /// Load crunch atlas data from a binary file
        /// @param filepath path to the file to open
        /// @param data [out] pointer to receive the data - must not be null
        static bool loadBinaryFile(const string &filepath, bool trimEnabled, bool rotateEnabled, CrunchAtlasData *data);

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

