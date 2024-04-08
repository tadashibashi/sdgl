#pragma once

namespace sdgl::graphics {
    struct Glyph;

    /// Font to render pre-rendered atlas of glyphs
    /// Currently, only Angel Code BMFont format is supported, and this implementation is built around it
    class BitmapFont
    {
    public:
        BitmapFont();
        ~BitmapFont();

        bool loadBMFont(const string &filepath);
        void free();

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
        void projectText(vector<Glyph> &glyphs, const string &text, uint maxWidth = 0, int horSpaceOffset = 0,
            int lineHeightOffset = 0, bool withKerning = true) const;
    private:
        struct Impl;
        struct Char;
        Impl *m;
    };
}

