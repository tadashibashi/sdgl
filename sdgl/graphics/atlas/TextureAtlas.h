#pragma once

#include <sdgl/graphics/Texture2D.h>
#include <sdgl/math/Rectangle.h>

namespace sdgl::graphics {
    class TextureAtlas {
    public:
        ~TextureAtlas();
        struct Frame {
            Rectangle_<int16> frame{};
            Vector2_<int16> offset{};
            Vector2_<int16> size{};
            bool rotated = false;
            Texture2D texture{};
        };

        /// Load a crunch file (binary format)
        /// @param filepath path to crunch map binary file
        /// @returns whether this function succeeded
        bool loadCrunch(const string &filepath);

        /// Get a frame from the atlas
        /// @returns frame for the index
        /// @throws std::out_of_range if the container does not have this index
        [[nodiscard]]
        const Frame &operator[](const string &frameName) const {
            return m_frames.at(frameName);
        }

        /// Clear loaded map from memory
        void clear();

    private:
        map<string, Frame> m_frames;
        vector<Texture2D> m_textures;
    };
}

