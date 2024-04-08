#include "BitmapFont.h"

#include <__filesystem/path.h>

#include "BMFontData.h"
#include "Glyph.h"

namespace sdgl::graphics {
    struct BitmapFont::Char
    {
        Rectangle_<uint16> frame;
        Vector2_<int16> offset;   ///< x: subtract from cursorX to get frame start X;
                                  ///< y: cursorY - base + yoffset = frame start Y
        int16 xadvance;           ///< horizontal length from cursor start to end
        Texture2D texture;      ///< gpu texture id
    };

    struct BitmapFont::Impl
    {
        string fontName;
        uint16 fontSize; ///< original size from generation
        uint16 base = 0;                               ///< Distance from top line to the glyph baseline == cursorY
        uint16 lineHeight = 0;
        vector<Texture2D> pages;
        map<uint, Char> chars;
        map<std::pair<uint, uint>, int> kernings;

        void unload()
        {
            for (auto &page : pages)
            {
                page.free();
            }

            chars.clear();
            pages.clear();
            kernings.clear();
            fontName.clear();
            fontSize = 0;
            base = 0;
        }
    };

    BitmapFont::BitmapFont() : m(new Impl)
    {
    }

    BitmapFont::~BitmapFont()
    {
        delete m;
    }

    bool BitmapFont::loadBMFont(const string &filepath)
    {
        BMFontData data;
        if (!BMFontData::fromFile(filepath, &data))
        {
            return false;
        }

        // load pages into texture
        auto parentFolder = std::filesystem::path(filepath).parent_path().string();

        vector<Texture2D> textures;
        textures.reserve(data.pages.size());

        // Page textures
        for (const auto &[id, file] : data.pages)
        {
            Texture2D texture;
            if (!texture.loadFile(format("{}/{}", parentFolder, file)))
            {
                return false;
            }

            textures.emplace_back(texture);
        }

        // Chars
        map<uint, Char> chars;
        for (const auto &c : data.chars)
        {
            chars.try_emplace(c.id,
                Char {
                    Rectangle_<uint16>{c.x, c.y, c.width, c.height},
                    Vector2_<int16>(c.xoffset, c.yoffset),
                    c.xadvance,
                    textures[c.page]
                }
            );
        }

        // Kernings
        map<std::pair<uint, uint>, int> kernings;
        for (const auto &k : data.kernings)
        {
            kernings.try_emplace(std::make_pair(k.first, k.second), k.amount);
        }

        m->pages.swap(textures);
        m->chars.swap(chars);
        m->kernings.swap(kernings);
        m->fontName = data.info.fontName;
        m->fontSize = data.info.fontSize;
        m->base = data.common.base;
        m->lineHeight = data.common.lineHeight;

        return true;
    }

    void BitmapFont::free()
    {
        m->unload();
    }

    bool BitmapFont::isLoaded() const
    {
        return !m->pages.empty();
    }

    const string &BitmapFont::fontName() const
    {
        return m->fontName;
    }

    void BitmapFont::projectText(vector<Glyph> &glyphs, const string &text, const uint maxWidth, const int horSpaceOffset,
                                 const int lineHeightOffset, const bool withKerning) const
    {
        glyphs.clear();
        if (text.empty() || !isLoaded())
        {
            return;
        }

        const auto base = m->base;

        // Get cursor starting point
        const auto &firstChar = m->chars.at(text[0]);
        auto cursor = Point(firstChar.offset.x, base);

        const auto &spaceChar = m->chars.at(' '); // FIXME: could possibly cache this?

        // Collect glyph data
        for (size_t charIdx = 0, size = text.size(); charIdx < size; )
        {
            // 1. project width, see if it fits
            // 2. push glyph, repeat

            if (text[charIdx] == '\n' || text[charIdx] == '\r')
            {
                // Explicit line break
                if (charIdx + 1 < size)
                {
                    cursor.x = m->chars.at(text[charIdx + 1]).offset.x;
                    cursor.y += m->lineHeight + lineHeightOffset;
                }
                ++charIdx;

            }
            else if (text[charIdx] == ' ')
            {
                // Space

                // FIXME: research if kerning for spaces is necessary?
                // See if kerning available to adjust wordWidth == this word's start point
                if (withKerning && charIdx > 0)
                {
                    const auto kernIt = m->kernings.find( std::make_pair(text[charIdx-1], ' ') );
                    if (kernIt != m->kernings.end())
                    {
                        // apply found kerning
                        cursor.x += kernIt->second;
                    }
                }

                // Check if we need to drop down a line
                if (maxWidth != 0 && cursor.x + spaceChar.xadvance + horSpaceOffset - spaceChar.offset.x > maxWidth)
                {
                    // drop down one line
                    cursor.x = spaceChar.offset.x;
                    cursor.y += m->lineHeight + lineHeightOffset;

                    // push glyph
                    glyphs.emplace_back(
                        spaceChar.frame,
                        Point(cursor.x + spaceChar.offset.x, cursor.y - base + spaceChar.offset.y),
                        spaceChar.texture
                    );
                }

                // Advance past space char
                cursor.x += spaceChar.xadvance;
                ++charIdx;
            }
            else
            {
                // For each word
                // find end of word
                size_t endWord = charIdx + 1;
                while (endWord < size && !std::isspace(text[endWord]))
                    ++endWord;

                // Push glyphs for word (track width to see if we need linebreak after)
                size_t wordWidth = 0; ///< acts as temporary x cursor offset for word
                for (size_t w = charIdx; w < endWord; ++w)
                {
                    // Get data for char
                    const auto &curChar = m->chars.at(text[w]);

                    // See if kerning available to adjust wordWidth == this word's start point
                    if (withKerning && w > 0)
                    {
                        const auto kernIt = m->kernings.find( std::make_pair(text[w-1], text[w]) );
                        if (kernIt != m->kernings.end())
                        {
                            // apply found kerning
                            wordWidth += kernIt->second;
                        }
                    }

                    // Push glyph for char
                    glyphs.emplace_back(
                        curChar.frame,
                        Point(cursor.x + (int)wordWidth + curChar.offset.x, cursor.y - base + curChar.offset.y),
                        curChar.texture
                    );

                    // Advance past current char
                    wordWidth += curChar.xadvance + horSpaceOffset;
                }

                // Check if we need a line break
                if (maxWidth != 0 && wordWidth + cursor.x > maxWidth)
                {
                    const auto minusX = cursor.x;
                    const auto plusY = m->lineHeight + lineHeightOffset;

                    // apply line break repositioning to all glyphs that were just pushed
                    for (auto i = charIdx; i<  endWord; ++i)
                    {
                        glyphs[i].destination.x -= minusX;
                        glyphs[i].destination.y += plusY;
                    }

                    // send cursor to the next line
                    cursor.x = m->chars.at(charIdx).offset.x;
                    cursor.y += plusY;
                }
                else
                {
                    // no line break, add word width
                    cursor.x += static_cast<int>(wordWidth);
                }

                // Move char counter to the end of word
                charIdx = endWord;
            }


        }

    }
}
