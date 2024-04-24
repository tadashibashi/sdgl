#pragma once

#include <sdgl/graphics/Frame.h>
#include <sdgl/graphics/Texture2D.h>

namespace sdgl {
    class TextureAtlas final : public Asset {
    public:
        ~TextureAtlas() override;

        /// Load a crunch file (binary format)
        /// @param filepath path to crunch map binary file
        /// @returns whether this function succeeded
        bool loadCrunch(const string &filepath);

        /// Load crunch data already in memory. Filepath is required for relative paths to load image files.
        /// @param filepath path to the original resource file e.g. "path/to/atlas.bin"
        /// @param fileBuffer in-memory data buffer containing file data
        bool loadCrunchMem(const string &filepath, const string &fileBuffer);

        /// Get a frame from the atlas
        /// @returns frame for the index
        /// @throws std::out_of_range if the container does not have this index
        [[nodiscard]]
        const Frame &operator[](const string &frameName) const {
            return m_frames.at(frameName);
        }

        /// Get a frame from the atlas
        /// @returns frame for the index
        /// @throws std::out_of_range if the container does not have this index
        [[nodiscard]]
        const Frame &at(const string &frameName) const
        {
            return m_frames.at(frameName);
        }

        /// Clear loaded map from memory
        void unload() override;

    private:
        map<string, Frame> m_frames;
        vector<Texture2D> m_textures;
    };
}

