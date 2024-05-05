#include "BitmapFont.h"

#include <filesystem>

#include "BMFontData.h"
#include "Glyph.h"

namespace sdgl {
    struct BitmapFont::Char
    {
        Rect<uint16> frame;
        Vec2<int16> offset;   ///< x: subtract from cursorX to get frame start X;
                                  ///< y: cursorY - base + yoffset = frame start Y
        int16 xadvance;           ///< horizontal length from cursor start to end
        const Frame &texture;     ///< texture frame
    };

    struct BitmapFont::Impl
    {
        string fontName;
        uint16 fontSize{};            ///< original size from generation
        uint16 base = 0;            ///< Distance from top line to the glyph baseline == cursorY
        uint16 lineHeight = 0;
        vector<Frame> pages;
        map<uint, Char> chars;
        map<std::pair<uint, uint>, int> kernings;
        bool ownsFrames = false;

        void unload()
        {
            if (ownsFrames)
            {
                for (auto &page : pages)
                {
                    page.texture.unload();
                }

                ownsFrames = false;
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

    bool BitmapFont::loadBMFont(const string &filepath, const TextureAtlas &textureAtlas, string_view textureRoot)
    {
        BMFontData data;
        if (!BMFontData::fromFile(filepath, &data))
        {
            return false;
        }

        return parseBMFontData(data, textureRoot, &textureAtlas);
    }

    bool BitmapFont::loadBMFont(const string &filepath)
    {
        BMFontData data;
        if (!BMFontData::fromFile(filepath, &data))
        {
            return false;
        }

        const auto parentPath = std::filesystem::path(filepath).parent_path();
        return parseBMFontData(data, parentPath.string(), nullptr);
    }

    bool BitmapFont::loadBMFontMem(const string &fileBuffer, const TextureAtlas &textureAtlas, string_view textureRoot)
    {
        BMFontData data;
        if (!BMFontData::fromBuffer(fileBuffer, &data))
        {
            return false;
        }

        return parseBMFontData(data, textureRoot, &textureAtlas);
    }

    bool BitmapFont::loadBMFontMem(const string &fileBuffer, const string &parentFolder)
    {
        BMFontData data;
        if (!BMFontData::fromBuffer(fileBuffer, &data))
        {
            return false;
        }

        return parseBMFontData(data, parentFolder, nullptr);
    }

    void BitmapFont::unload()
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

    Point BitmapFont::projectText(vector<Glyph> &glyphs, const string &text, const uint maxWidth, const int horSpaceOffset,
                                 const int lineHeightOffset, const bool withKerning) const
    {
        glyphs.clear();
        if (text.empty() || !isLoaded())
        {
            return {};
        }

        Point cursorMax;

        const auto base = m->base;

        // Get cursor starting point
        const auto &firstChar = m->chars.at(text[0]);
        auto cursor = Point(-firstChar.offset.x, base);

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
                    cursor.x = -m->chars.at(text[charIdx + 1]).offset.x;
                    cursor.y += m->lineHeight + lineHeightOffset;
                }
                ++charIdx;

            }
            else if (text[charIdx] == ' ')
            {
                // Space

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

                // push glyph
                glyphs.emplace_back(
                    spaceChar.frame,
                    Point(cursor.x + (int)spaceChar.offset.x, cursor.y - (int)base + (int)spaceChar.offset.y),
                    spaceChar.texture
                );

                // Advance cursor

                // Rows should not start with spaces, so we won't indent for any space glyph exceeding maxWidth
                // (Leave code here in case that behavior might be useful at some time...)

                // Check if we need a line break
                // if (charIdx + 1 < size && maxWidth != 0 && cursor.x + spaceChar.xadvance + horSpaceOffset + spaceChar.offset.x > maxWidth)
                // {
                //     // Drop down one line
                //     cursor.x = -m->chars.at(text[charIdx + 1]).offset.x;
                //     cursor.y += m->lineHeight + lineHeightOffset;
                // }
                // else
                {
                    // Just advance past space char ->
                    cursor.x += spaceChar.xadvance + horSpaceOffset;

                    // don't account for greatestWidth here since it's empty space
                }

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
                int wordWidth = 0; ///< acts as temporary x cursor offset for word
                const auto glyphIdx = glyphs.size();

                for (size_t w = charIdx; w < endWord; ++w)
                {
                    // Get data for char
                    const auto &curChar = m->chars.at(text[w]);

                    // See if kerning available to adjust wordWidth == this word's start point
                    if (withKerning && w > 0 && glyphs.back().destination.x < wordWidth + curChar.offset.x) // second check makes sure glyph isn't at the beginning of line where kerning not applied
                    {
                        const auto kernIt = m->kernings.find( std::make_pair<uint, uint>(text[w-1], text[w]) );
                        if (kernIt != m->kernings.end())
                        {
                            // apply found kerning
                            wordWidth += kernIt->second;
                        }
                    }

                    // Push glyph for char
                    glyphs.emplace_back(
                        curChar.frame,
                        Point(cursor.x + wordWidth + (int)curChar.offset.x, cursor.y - (int)base + (int)curChar.offset.y),
                        curChar.texture
                    );

                    // Advance past current char
                    wordWidth += curChar.xadvance + horSpaceOffset;
                }

                // Check if we need a line break
                if (maxWidth != 0 && glyphs.back().destination.x + glyphs.back().source.w > maxWidth)
                {
                    const auto plusX = -glyphs.at(glyphIdx).destination.x - (int)m->chars.at(text[charIdx]).offset.x;
                    const auto plusY = (int)m->lineHeight + lineHeightOffset;

                    // apply line break repositioning to all glyphs that were just pushed
                    for (auto i = glyphIdx, glyphSize = glyphs.size(); i < glyphSize; ++i)
                    {
                        glyphs[i].destination.x += plusX;
                        glyphs[i].destination.y += plusY;
                    }

                    // send cursor to the next line if there's more text to come
                    if (endWord < size)
                    {
                        cursor.x += plusX + wordWidth;
                        cursor.y += plusY;
                    }
                }
                else
                {
                    // no line break, add word width
                    cursor.x += static_cast<int>(wordWidth);

                }

                if (cursor.x > cursorMax.x)
                    cursorMax.x = cursor.x;
                if (cursor.y > cursorMax.y)
                    cursorMax.y = cursor.y;

                // Move char counter to the end of word
                charIdx = endWord;
            }

        }

        return cursorMax;

    }

    bool BitmapFont::parseBMFontData(const BMFontData &data, string_view textureRoot, const TextureAtlas *atlas)
    {
        // load pages into texture
        auto parentFolder = std::filesystem::path(textureRoot);

        vector<Frame> textureFrames;
        textureFrames.reserve(data.pages.size());

        // Get page textures
        if (atlas) // from atlas, if provided
        {
            for (const auto &[id, file] : data.pages)
            {
                textureFrames.emplace_back(atlas->at( (parentFolder / file).replace_extension() ));
            }
        }
        else      // from direct files, if atlas not provided
        {
            for (const auto &[id, file] : data.pages)
            {
                Texture2D texture;
                if (!texture.loadFile(parentFolder / file))
                {
                    return false;
                }

                const auto textureSize = static_cast<Vec2<int16>>(texture.size());
                textureFrames.emplace_back(Frame{
                    Rect<int16>{0, 0, textureSize.x , textureSize.y},
                    Vec2<int16> {},
                    textureSize,
                    false,
                    texture
                });
            }
        }


        // Parse chars
        map<uint, Char> chars;
        for (const auto &c : data.chars)
        {
            chars.try_emplace(c.id,
                Char {
                    Rect<uint16>{c.x, c.y, c.width, c.height},
                    Vec2<int16>(c.xoffset, c.yoffset),
                    c.xadvance,
                    textureFrames[c.page]
                }
            );
        }

        // Parse kernings
        map<std::pair<uint, uint>, int> kernings;
        for (const auto &k : data.kernings)
        {
            kernings.try_emplace(std::make_pair(k.first, k.second), k.amount);
        }

        m->pages.swap(textureFrames);
        m->chars.swap(chars);
        m->kernings.swap(kernings);
        m->fontName = data.info.fontName;
        m->fontSize = data.info.fontSize;
        m->base = data.common.base;
        m->lineHeight = data.common.lineHeight;
        m->ownsFrames = !static_cast<bool>(atlas);

        return true;
    }
}
