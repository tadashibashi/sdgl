#pragma once
#include <sdgl/sdglib.h>

namespace sdgl {
    /// Manages the loading and storage of an AngelCode BMFont file
    /// Format documentation can be found at:
    /// https://www.angelcode.com/products/bmfont/doc/file_format.html
    struct BMFontData
    {
        /// "info" tag struct - it holds info on how the font was generated
        struct Info
        {
            /// Bitfield used to describe attributes about the font's origin
            struct Attributes
            {
                enum Enum : ubyte
                {
                    Smooth = 1,            ///< anti-aliasing algorithm used
                    Unicode = 1 << 1,      ///< unicode character set used
                    Italic = 1 << 2,       ///< italic font style
                    Bold = 1 << 3,         ///< bold font style
                    FixedHeight = 1 << 4,  ///< fixed height font
                };
            };

            short fontSize;            ///< Font size in pixels
            Attributes::Enum bitField; ///< Originating font attributes stored as a bit field
            ubyte charSet;             ///< The name of the OEM charset used (when not unicode)
            ushort stretchH;           ///< The font height stretch in percentage. 100 (100%) means no stretch.
            ubyte aa;                  ///< Supersampling level used. 1 means no supersampling was used.
            ubyte paddingUp;           ///< upward padding for each character
            ubyte paddingRight;        ///< rightward padding for each character
            ubyte paddingDown;         ///< downward padding for each character
            ubyte paddingLeft;         ///< leftward padding for each character
            ubyte spacingHoriz;        ///< horizontal spacing for each character
            ubyte spacingVert;         ///< vertical spacing for each character
            ubyte outline;             ///< outline thickness for the characters
            string fontName;           ///< name of the originating truetype font
        } info;

        /// Block type 2
        struct Common
        {
            ushort lineHeight;        ///< distance in pixels between each line of text
            ushort base;              ///< number of pixels from the absolute top of the line to the baseline of the characters
            ushort scaleW;            ///< the width of the texture, normally used to scale the x position of the character image
            ushort scaleH;            ///< the height of the texture, normally used to scale the y position of the character image
            ushort pages;             ///< the number of texture pages included in the font
            ubyte bitField;           ///< bits 0-6 reserved, bit 7: packed
            ubyte alphaChnl;          ///< set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if it is set to zero, and 4 if it is set to one
            ubyte redChnl;            ///< set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if it is set to zero, and 4 if it is set to one
            ubyte greenChnl;          ///< set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if it is set to zero, and 4 if it is set to one
            ubyte blueChnl;           ///< set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if it is set to zero, and 4 if it is set to one
        } common;

        /// Block type 3: This tag gives the name of a texture file. There is one for each page in the font.
        struct Page
        {
            uint id;           ///< page identifier, same as index in pages vector
            string file;       ///< texture file name
        };
        vector<Page> pages;

        /// Block type 4: This tag describes one character in the font. There is one for each included character in the font.
        struct Char
        {
            uint id;                           ///< character id, e.g. ascii/unicode number
            uint16 x, y, width, height;        ///< source position and size in texture
            int16 xoffset, yoffset;            ///< offset to apply when when displying to match original
            int16 xadvance;                    ///< how much the cursor position should be advanced after drawing the character
            ubyte page;                        ///< page index of source texture
            ubyte chnl;                        ///< texture channel where the character image is found (1 = blue, 2 = green, 4 = red, 8 = alpha, 15 = all channels)
        };
        vector<Char> chars;

        /// Block type 5: The kerning information used to adjust the distance between certain characters, e.g. some characters should be placed closer to each other than others.
        struct KerningPair
        {
            uint first;     ///< first character id
            uint second;    ///< second character id
            int16 amount;   ///< how much the x psoition should be adjusted when drawing the second character immediately following the first
        };
        vector<KerningPair> kernings;

        /// Read bmfont data into the object. Only supports the text binary version.
        /// @param filepath path to the bmfont file to open
        /// @param data structure to receive the data
        /// @returns whether operation succeeded
        static bool fromFile(const string &filepath, BMFontData *outData);

        /// Read bmfont data into the object from an in-memory buffer. Only supports the binary file version.
        /// @param buffer contains the bmfont data
        /// @param outData structure to receive the data
        /// @return whether operation succeeded
        static bool fromBuffer(const string &buffer, BMFontData *outData);
    };
}
