#include "BMFontData.h"

#include <sdgl/assert.h>
#include <sdgl/logging.h>
#include <sdgl/io/io.h>
#include <sdgl/io/BufferView.h>

/// Helper to read a field from a BufferView for BMFont read functions. Invokes a `return
/// false` on error, so it should be called in a function with a bool return type.
#define BMFONT_READ(view, field) do { if ((view).read(field) != sizeof(field)) { \
    SDGL_ERROR("Failed to read {}", #field); \
    return false; \
}} while(0)

namespace sdgl {
    bool BMFontData::fromFile(const string &filepath, BMFontData *data)
    {
        SDGL_ASSERT(data);

        string buffer;
        if (!io::readFile(filepath, &buffer))
        {
            return false;
        }

        return fromBuffer(buffer, data);
    }

    enum class BMFontType
    {
        None,
        BinaryV3,
        Text,
        Xml
    };

    /// Check id bytes for BMFontType
    static BMFontType getBMFontType(const string &buffer)
    {
        if (buffer.size() < 6)
        {
            SDGL_ERROR("File type is not a BMFont format");
            return BMFontType::None;
        }

        if (buffer[0] == 'B' &&
            buffer[1] == 'M' &&
            buffer[2] == 'F')
        {
            if (buffer[3] != 3)
            {
                SDGL_ERROR("Unsupported BMFont binary format: `{}`, please export version 3",
                    static_cast<int>(buffer[3]));
                return BMFontType::None;
            }

            return BMFontType::BinaryV3;
        }

        if (buffer[0] == 'i' &&
            buffer[1] == 'n' &&
            buffer[2] == 'f' &&
            buffer[3] == 'o')
        {
            return BMFontType::Text;
        }

        if (buffer[0] == '<' &&
            buffer[1] == 'f' &&
            buffer[2] == 'o' &&
            buffer[3] == 'n' &&
            buffer[4] == 't' &&
            buffer[5] == '>')
        {
            return BMFontType::Xml;
        }

        SDGL_ERROR("File type is not a BMFont format");
        return BMFontType::None;
    }

    static bool parseBinaryV3(const string &buffer, BMFontData *outData)
    {
        auto view = io::BufferView(buffer);
        view.move(4); // move past file identifier

        BMFontData bmfont;

        // Info: block 1
        if (const auto blockId = view.getOr<ubyte>(); blockId != 1)
        {
            SDGL_ERROR("Invalid BMFont v3 binary file: expected block number 1, but got {}", blockId);
            return false;
        }
        if (uint blockSize = 0; view.read(blockSize) != 4 || blockSize < 14)
        {
            SDGL_ERROR("Invalid BMFont v3 info block: expected block 1 to be at least 14 bytes, but it was {} bytes",
                blockSize);
            return false;
        }
        BMFONT_READ(view, bmfont.info.fontSize);
        BMFONT_READ(view, (ubyte &)bmfont.info.bitField);
        BMFONT_READ(view, bmfont.info.charSet);
        BMFONT_READ(view, bmfont.info.stretchH);
        BMFONT_READ(view, bmfont.info.aa);
        BMFONT_READ(view, bmfont.info.paddingUp);
        BMFONT_READ(view, bmfont.info.paddingRight);
        BMFONT_READ(view, bmfont.info.paddingDown);
        BMFONT_READ(view, bmfont.info.paddingLeft);
        BMFONT_READ(view, bmfont.info.spacingHoriz);
        BMFONT_READ(view, bmfont.info.spacingVert);
        BMFONT_READ(view, bmfont.info.outline);
        view.read(bmfont.info.fontName);

        // Common: block 2
        if (const auto blockId = view.getOr<ubyte>(); blockId != 2)
        {
            SDGL_ERROR("Invalid BMFont v3 binary file: expected block number 2, but got {}", blockId);
            return false;
        }

        if (uint blockSize = 0; view.read(blockSize) != 4 || blockSize != 15)
        {
            SDGL_ERROR("Invalid BMFont v3 common block: expected block 2 to be 15 bytes, but got {}", blockSize);
            return false;
        }
        BMFONT_READ(view, bmfont.common.lineHeight);
        BMFONT_READ(view, bmfont.common.base);
        BMFONT_READ(view, bmfont.common.scaleW);
        BMFONT_READ(view, bmfont.common.scaleH);
        BMFONT_READ(view, bmfont.common.pages);
        BMFONT_READ(view, bmfont.common.bitField);
        BMFONT_READ(view, bmfont.common.alphaChnl);
        BMFONT_READ(view, bmfont.common.redChnl);
        BMFONT_READ(view, bmfont.common.greenChnl);
        BMFONT_READ(view, bmfont.common.blueChnl);

        // Pages: block 3
        if (const auto blockId = view.getOr<ubyte>(); blockId != 3)
        {
            SDGL_ERROR("Invalid BMFont v3 binary file: expected block number 3, but got {}", blockId);
            return false;
        }

        uint pagesBlockSize = 0;
        if (view.read(pagesBlockSize) != 4 || pagesBlockSize == 0)
        {
            SDGL_ERROR("Invalid BMFont v3 pages block size: {}", pagesBlockSize);
            return false;
        }

        // Find the length of 1 string (including null-term), all are equal-length
        int pageStrLen = 0;
        {
            bool termFound = false;

            while (pageStrLen < pagesBlockSize)
            {
                if (!termFound)
                {
                    if (view.peek(pageStrLen) == 0)
                    {
                        termFound = true;
                    }

                    ++pageStrLen;
                }
                else
                {
                    if (view.peek(pageStrLen) != 0)
                    {
                        break;
                    }

                    ++pageStrLen;
                }
            }
        }

        // Ensure pages block is evenly divisible by the page string length we just got
        if (pagesBlockSize % pageStrLen != 0)
        {
            SDGL_ERROR("Invalid BMFont v3 page filepath string length, block must be evenly divisible by this number,"
                " but got: block size {} with string length {}", pagesBlockSize, pageStrLen);
            return false;
        }

        // Visit each page
        for (auto pagePtr = 0; pagePtr < pagesBlockSize; pagePtr += pageStrLen)
        {
            string pageImagePath;
            view.readFixedString(pageImagePath, pageStrLen - 1);

            bmfont.pages.emplace_back(BMFontData::Page{
                .id = static_cast<uint>(bmfont.pages.size()),
                .file = pageImagePath,
            });
        }

        // Chars: block 4
        if (const auto blockId = view.getOr<ubyte>(); blockId != 4)
        {
            SDGL_ERROR("Invalid BMFont v3 binary file: expected block number 4, but got {}", blockId);
            return false;
        }

        uint charsBlockSize = 0;
        if (charsBlockSize = 0; view.read(charsBlockSize) != 4 || charsBlockSize % 20 != 0) // one Char is 20 bytes
        {
            SDGL_ERROR("Invalid BMFont v3 chars block size, must be divisible by 20 and > 0, but got: {}", charsBlockSize);
            return false;
        }

        for (auto charsPtr = 0; charsPtr < charsBlockSize; charsPtr += 20)
        {
            BMFontData::Char curChar{};
            BMFONT_READ(view, curChar.id);
            BMFONT_READ(view, curChar.x);
            BMFONT_READ(view, curChar.y);
            BMFONT_READ(view, curChar.width);
            BMFONT_READ(view, curChar.height);
            BMFONT_READ(view, curChar.xoffset);
            BMFONT_READ(view, curChar.yoffset);
            BMFONT_READ(view, curChar.xadvance);
            BMFONT_READ(view, curChar.page);
            BMFONT_READ(view, curChar.chnl);

            SDGL_ASSERT(curChar.chnl == 15, "sdgl only supports full-channel rendering of fonts");

            bmfont.chars.emplace_back(curChar);
        }

        // Kerning Pairs: block 5 (only available if there are any kerning pairs with `amount` differing from 0)
        if (view.position() < view.size())
        {
            if (const auto blockId = view.getOr<ubyte>(); blockId != 5)
            {
                SDGL_ERROR("Invalid BMFont v3 binary file: expected block number 5, but got {}", blockId);
                return false;
            }

            uint kerningBlockSize = 0;
            if (view.read(kerningBlockSize) != 4 || kerningBlockSize % 10 != 0)
            {
                SDGL_ERROR("Invalid BMFont v3 kerning block size, must be divisible by 10 and > 0, but got: {}",
                    kerningBlockSize);
                return false;
            }

            for (uint kerningPtr = 0; kerningPtr < kerningBlockSize; kerningPtr += 10)
            {
                BMFontData::KerningPair kerning{};
                BMFONT_READ(view, kerning.first);
                BMFONT_READ(view, kerning.second);
                BMFONT_READ(view, kerning.amount);

                bmfont.kernings.emplace_back(kerning);
            }
        }

        if (view.position() != view.size())
        {
            SDGL_WARN("BMFont v3 binary file may have been read incorrectly: "
                "bytes read does not equal total bytes: {} READ, {} SIZE", view.position(), view.size());
        }

        *outData = std::move(bmfont);
        return true;
    }

    bool BMFontData::fromBuffer(const string &buffer, BMFontData *outData)
    {
        SDGL_ASSERT(outData);

        switch(getBMFontType(buffer))
        {
            case BMFontType::None:
                return false;
            case BMFontType::Xml:
                SDGL_ERROR("XML BMFont file types are not supported yet.");
                return false;
            case BMFontType::Text:
                SDGL_ERROR("Text BMFont file types are not supported yet.");
                return false;
            case BMFontType::BinaryV3:
                return parseBinaryV3(buffer, outData);
            default:
                SDGL_ERROR("Invalid BMFont type enumeration");
                return false;
        }
    }
}
