#pragma once
#include "BitmapFont.h"
#include "Glyph.h"

namespace sdgl::graphics {

    /// Stores a piece of text to be rendered and caches a list of glyphs according to the set parameters
    class FontText
    {
    public:
        struct Config
        {
            BitmapFont *font = nullptr;
            uint maxWidth = 0;
            bool useKerning = true;
            int horizSpaceOffset = 0;
            int lineHeightOffset = 0;
        };

        FontText();
        explicit FontText(const Config &config, string_view text = "");
        explicit FontText(BitmapFont *font, string_view text = "", uint maxWidth = 0,
                          bool useKerning = true, int horSpaceOffset = 0, int lineHeightOffset = 0);

        [[nodiscard]]
        const BitmapFont *getFont() const { return m_font; }
        FontText &font(BitmapFont *value);

        [[nodiscard]]
        const string &getText() const { return m_text; }
        FontText &setText(string_view value);

        [[nodiscard]]
        int getHorizSpaceOffset() const { return m_horSpaceOffset; }
        FontText &setHorizSpaceOffset(int value);

        /// Get max width in pixels; 0 means no max width limit
        [[nodiscard]]
        uint getMaxWidth() const { return m_maxWidth; }

        /// Set max width in pixels; 0 means no max width limit
        FontText &setMaxWidth(uint value);

        /// Use character-specific kerning offset when rendering (default: true)
        [[nodiscard]]
        bool useKerning() const { return m_useKerning; }

        /// Set whether to use character-specific kerning offset when rendering (default: true)
        FontText &useKerning(bool value);

        /// Get line height pixel offset; this gets added to the font's line height (default: 0)
        [[nodiscard]]
        int lineHeightOffset() const { return m_lineHeightOffset; }
        /// Set the line height pixel offset; this gets added to the font's line height (default: 0)
        FontText &lineHeightOffset(int value);

        /// Get the current glyphs to be rendered, gets updated automatically when a font is available
        /// and text is not empty.
        [[nodiscard]]
        const vector<Glyph> &glyphs() const { return m_glyphs; }

        /// Set the number of chars to show when SpriteBatch or some other system renders this object.
        /// This is useful for dialog that is revealed gradually.
        /// @note make sure to check glyphs().size() instead of text().size(), since invisible characters like
        ///       carriage returns / line feeds get counted as a string character, but is not counted as a glyph.
        [[nodiscard]]
        size_t textProgress() const { return m_textProgress; }
        /// Set text progress - the number of chars to show when SpriteBatch or some other system renders
        /// this object. This is useful for dialog that is revealed gradually.
        /// @note make sure to check glyphs().size() insteadof text().size(), since invisible characters like
        ///       carriage returns / line feeds get counted as a string character, but is not counted as a glyph.
        FontText &textProgress(size_t value) { m_textProgress = value; return *this; }

    private:
        void updateGlyphs();

        vector<Glyph> m_glyphs;
        BitmapFont *m_font;
        string m_text;
        uint m_maxWidth;
        uint m_textProgress;
        bool m_useKerning;
        int m_horSpaceOffset;
        int m_lineHeightOffset;
    };

} // sdgl::graphics
