#include "FontText.h"


namespace sdgl {
    FontText::FontText() : m_glyphs(), m_font(nullptr), m_text(), m_maxWidth(0), m_textProgress(0), m_useKerning(true),
                          m_horSpaceOffset(0), m_lineHeightOffset(0), m_shouldUpdateSize(false)
    {}

    FontText::FontText(const Config &config, const string_view text) : m_glyphs(), m_font(config.font), m_text(text),
        m_maxWidth(config.maxWidth), m_textProgress(text.length()), m_useKerning(config.useKerning),
        m_horSpaceOffset(config.horizSpaceOffset), m_lineHeightOffset(config.lineHeightOffset), m_shouldUpdateSize(false)
    {
        updateGlyphs();
    }

    FontText::FontText(BitmapFont *font, const string_view text, const uint maxWidth, const bool useKerning,
                       const int horSpaceOffset, const int lineHeightOffset) :
        m_glyphs(), m_font(font), m_text(text), m_maxWidth(maxWidth), m_textProgress(text.length()),
        m_useKerning(useKerning), m_horSpaceOffset(horSpaceOffset), m_lineHeightOffset(lineHeightOffset)
    {
        updateGlyphs();
    }

    FontText &FontText::font(BitmapFont *value)
    {
        if (value != m_font)
        {
            m_font = value;
            updateGlyphs();
        }
        return *this;
    }

    FontText &FontText::setText(const string_view value)
    {
        if (m_text != value)
        {
            m_text = value;
            m_textProgress = value.length();
            updateGlyphs();
        }
        return *this;
    }

    FontText &FontText::setHorizSpaceOffset(const int value)
    {
        if (value != m_horSpaceOffset)
        {
            m_horSpaceOffset = value;
            updateGlyphs();
        }
        return *this;
    }

    FontText &FontText::setMaxWidth(uint value)
    {
        if (m_maxWidth != value)
        {
            m_maxWidth = value;
            updateGlyphs();
        }
        return *this;
    }

     FontText &FontText::textProgress(size_t value)
     {
        if (m_textProgress != value)
        {
            m_textProgress = value;
            m_shouldUpdateSize = true;
        }

        return *this;
    }

    FontText &FontText::useKerning(bool value)
    {
        if (m_useKerning != value)
        {
            m_useKerning = value;
            updateGlyphs();
        }
        return *this;
    }

    FontText &FontText::lineHeightOffset(int value)
    {
        if (m_lineHeightOffset != value)
        {
            m_lineHeightOffset = value;
            updateGlyphs();
        }
        return *this;
    }

    Point FontText::currentSize() const
    {
        if (m_shouldUpdateSize)
        {
            updateCurrentSize();
            m_shouldUpdateSize = false;
        }
        return m_curSize;
    }

    void FontText::updateGlyphs()
    {
        if (m_font)
        {
            m_font->projectText(m_glyphs, m_text, m_maxWidth, m_horSpaceOffset, m_lineHeightOffset,
                                m_useKerning);
        }
        else
        {
            m_glyphs.clear();
        }

        m_shouldUpdateSize = true;
    }

    void FontText::updateCurrentSize() const
    {
        if (m_textProgress == 0 || m_glyphs.empty())
        {
            m_curSize = {};
            return;
        }

        Point size;
        for (const auto &g : m_glyphs)
        {
            auto width = g.destination.x + g.source.w;
            auto height = g.destination.y + g.source.h;
            if (width > size.x)
                size.x = width;
            if (height > size.y)
                size.y = height;
        }

        m_curSize = size;
    }

} // sdgl::graphics
