#include "BMFontData.h"
#include <sdgl/io/io.h>
#include <sdgl/io/BufferView.h>

namespace sdgl::graphics {
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

    static bool parseBinaryV3(const string &buffer, BMFontData *data)
    {
        auto view = io::BufferView(buffer);
        view.move(4); // move past file identifier

        BMFontData temp;

        if (const auto blockId = view.read<ubyte>(); blockId != 1)
        {
            SDGL_ERROR("Invalid BMFont v3 binary file: expected block number 1, but got {}", blockId);
            return false;
        }


        return true;
    }

    bool BMFontData::fromBuffer(const string &buffer, BMFontData *data)
    {
        SDGL_ASSERT(data);
        auto type = getBMFontType(buffer);

        switch(type)
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
                return parseBinaryV3(buffer, data);
            default:
                SDGL_ERROR("Invalid BMFont type enumeration");
                return false;
        }
    }
}
