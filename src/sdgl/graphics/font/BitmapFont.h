#pragma once
#include <sdgl/Asset.h>
#include <sdgl/graphics/atlas/TextureAtlas.h>

namespace sdgl {
    struct BMFontData;
    struct Glyph;

    /// Font to render pre-rendered atlas of glyphs
    /// Currently, only Angel Code BMFont format is supported, and this implementation is built around it
    class BitmapFont final : public Asset
    {
    public:
        BitmapFont();
        ~BitmapFont() override;

        bool loadBMFont(const string &filepath, const TextureAtlas &textureAtlas, string_view textureRoot);
        bool loadBMFont(const string &filepath);

        /// Load from AngelCode BMFont data already in memory - textures are received from a texture atlas
        /// @param fileBuffer   in-memory data of AngelCode BMFont file
        /// @param textureAtlas texture atlas to load textures from
        /// @param textureRoot  parent path within the atlas where the texture keys are located
        ///
        bool loadBMFontMem(const string &fileBuffer, const TextureAtlas &textureAtlas,
            string_view textureRoot);

        /// Load from AngelCode BMFont data already in memory
        bool loadBMFontMem(const string &fileBuffer, const string &parentFolder);

        void unload() override;

        [[nodiscard]]
        bool isLoaded() const;

        [[nodiscard]]
        const string &fontName() const;

        /// Get projection rectangles for each character in a text string
        /// @param glyphs [out]  vector to receive glyph projection data; each position is relative to {0, 0}
        /// @param text         text string to project
        /// @param maxWidth     max width of text rectangle to draw, where the next glyphs will begin on the next line
        ///                     (if there is only one glyph on a line and it exceeds maxWidth, it will be drawn anyway);
        ///                     set to zero for no maxWidth limitation
        /// @param horSpaceOffset   value to offset horizontal spacing between each character
        /// @param lineHeightOffset value to offset lineHeight provided in the font (default: 0, no modification)
        /// @param withKerning  whether to apply horizontal kerning spacing rules (default: true)
        ///
        /// @note This function is somewhat expensive, please call only when text changes
        /// @returns greatest extent of the cursor for any non-white space character
        ///  (cursor.y goes to the baseline, but not full height of a character; cursor x-position goes beyond last character)
        Point projectText(vector<Glyph> &glyphs, const string &text, uint maxWidth = 0, int horSpaceOffset = 0,
            int lineHeightOffset = 0, bool withKerning = true) const;
    private:

        /// Parse bmfont where font textures are retrieved from a texture atlas
        /// @param data successfully loaded bmfont data object
        /// @param textureRoot parent path of where texture files are found - if atlas is not null,
        ///                    it indicates the parent path of where the texture keys are located
        /// @param atlas texture atlas to get textures from
        bool parseBMFontData(const BMFontData &data, string_view textureRoot, const TextureAtlas *atlas);
        struct Impl;
        struct Char;
        Impl *m;
    };
}

