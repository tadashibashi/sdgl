#include "TextureAtlas.h"

#include <filesystem>
#include <sdgl/io/io.h>

#include "CrunchAtlasData.h"

namespace sdgl {

    TextureAtlas::~TextureAtlas()
    {
        unload();
    }

    bool TextureAtlas::loadCrunch(const string &filepath)
    {
        // Load file
        string fileBuffer;
        if (!io::readFile(filepath, &fileBuffer))
        {
            return false;
        }

        return loadCrunchMem(filepath, fileBuffer);
    }

    bool TextureAtlas::loadCrunchMem(const string &filepath, const string &fileBuffer)
    {
       // Load crunch map
        CrunchAtlasData data;
        if (!CrunchAtlasData::loadBinary(fileBuffer, true, true, &data))
        {
            return false;
        }

        try
        {
            // Get data from crunch map
            vector<Texture2D> textures;              ///< texture 2D to collect
            textures.reserve(data.textures.size());
            map<string, Frame> frames;               ///< frames for each texture to collect

            auto path = std::filesystem::path(filepath);
            auto parentPath = path.parent_path();

            for (const auto &[texFileName, texImages] : data.textures)
            {
                // Load texture for atlas page
                Texture2D curTexture;
                if (!curTexture.loadFile( (parentPath / texFileName).string() + ".png" ))
                {
                    for (auto &t : textures)
                        t.unload();
                    return false;
                }

                textures.emplace_back(curTexture);

                // Get frames for this texture
                for (const auto &curImage : texImages)
                {
                    // frame names should be unique, but `try_emplace` is used anyway just in case
                    frames.try_emplace(curImage.name, Frame {
                        .frame = {curImage.x, curImage.y,
                            curImage.rotated ? curImage.height : curImage.width, curImage.rotated ? curImage.width : curImage.height},
                        .offset = {curImage.frameX, curImage.frameY},
                        .size = {curImage.frameWidth, curImage.frameHeight},
                        .rotated = curImage.rotated > 0,
                        .texture = curTexture
                    });
                }
            }

            // Done commit changes
            // Clear any existing textures
            if (!m_textures.empty())
            {
                for (auto &t : m_textures)
                    t.unload();
            }

            m_textures.swap(textures);
            m_frames.swap(frames);
            return true;
        }
        catch(const std::exception &e)
        {
            SDGL_ERROR("Failed to load crunch file: {}", e.what());
            return false;
        }
        catch(...)
        {
            SDGL_ERROR("Failed to load crunch file: unknown error");
            return false;
        }
    }

    void TextureAtlas::unload()
    {
        for (auto &texture : m_textures)
        {
            texture.unload();
        }

        m_textures.clear();
        m_frames.clear();
    }
}
