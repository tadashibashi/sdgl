#include "CrunchAtlasData.h"

#include <sdgl/assert.h>
#include <sdgl/io/BufferView.h>


/// Helper to read a field from a BufferView for BMFont read functions. Invokes a `return
/// false` on error, so it should be called in a function with a bool return type.
#define CRUNCH_READ(view, field) do { if ((view).read(field) != sizeof(field)) { \
    SDGL_ERROR("CrunchAtlasData failed to read {}", #field); \
    return false; \
}} while(0)

namespace sdgl {

    bool CrunchAtlasData::loadBinary(const string &buffer, bool trimEnabled, bool rotateEnabled,
        CrunchAtlasData *outData)
    {
       auto view = io::BufferView(buffer, io::Endian::Little);

        int16 numTextures;
        CRUNCH_READ(view, numTextures);
        SDGL_ASSERT(numTextures > 0);

        CrunchAtlasData crunchAtlas;
        for (int16 texIdx = 0; texIdx < numTextures; ++texIdx) // for each texture page in atlas
        {
            Texture texture;
            view.read(texture.name);
            SDGL_ASSERT(!texture.name.empty());

            int16 numImages;
            CRUNCH_READ(view, numImages);
            SDGL_ASSERT(numImages > 0);

            for (int16 imageIdx = 0; imageIdx < numImages; ++imageIdx) // for each sub-image in texture
            {
                Image image;
                view.read(image.name);
                SDGL_ASSERT(!image.name.empty());

                CRUNCH_READ(view, image.x);
                CRUNCH_READ(view, image.y);
                CRUNCH_READ(view, image.width);
                CRUNCH_READ(view, image.height);

                if (trimEnabled)
                {
                    CRUNCH_READ(view, image.frameX);
                    CRUNCH_READ(view, image.frameY);
                    CRUNCH_READ(view, image.frameWidth);
                    CRUNCH_READ(view, image.frameHeight);
                }
                else
                {
                    image.frameX = image.x;
                    image.frameY = image.y;
                    image.frameWidth = image.width;
                    image.frameHeight = image.height;
                }

                if (rotateEnabled)
                {
                    CRUNCH_READ(view, image.rotated);
                }
                else
                {
                    image.rotated = 0;
                }

                texture.images.emplace_back(image);
            }

            crunchAtlas.textures.emplace_back(texture);
        }

        SDGL_ASSERT(view.position() == view.size(),
            "Number of bytes read should be same as size of buffer");
        SDGL_ASSERT(!crunchAtlas.textures.empty(), "Atlas should have received at least one texture");

        // done, commit results
        *outData = std::move(crunchAtlas);
        return true;
    }
}

