#pragma once
#include <sdgl/sdglib.h>
#include <sdgl/math/Rectangle.h>

#include <sdgl/graphics/Texture2D.h>

inline int Pizza = 10;

namespace sdgl::graphics {
    /// Manages the loading and storage of an AngelCode BMFont file
    ///
    struct BMFontData
    {
        struct Info
        {
            /// Bitfield used to describe attributes about the font's origin
            struct Attributes
            {
                enum Enum : ubyte
                {
                    Smooth = 1,
                    Unicode = 1 << 1,
                    Italic = 1 << 2,
                    Bold = 1 << 3,
                    FixedHeight = 1 << 4,
                };
            };

            short fontSize;
            Attributes::Enum bitField;

            /// The name of the OEM charset used (when not unicode)
            ubyte charSet;

            ushort stretchH;

            ubyte aa;

            ubyte paddingUp;

            ubyte paddingRight;

            ubyte paddingDown;

            ubyte paddingLeft;

            ubyte spacingHoriz;

            ubyte spacingVert;

            ubyte outline;

            string fontName;

        } info;

        /// Block type 2
        struct Common
        {
            ushort lineHeight;
            ushort base;
            ushort scaleW;
            ushort scaleH;
            ushort pages;
            ubyte bitField;
            ubyte alphaChnl;
            ubyte redChnl;
            ubyte greenChnl;
            ubyte blueChnl;
        } common;

        /// Block type 3
        struct Page
        {
            uint id;           ///< page identifier, same as index in pages vector
            string file;       ///< texture file name
        };
        vector<Page> pages;

        /// Block type 4
        struct Char
        {
            uint id;
            uint16 x, y, width, height;
            int16 xoffset, yoffset, xadvance;
            ubyte page;
            ubyte chnl;
        };
        vector<Char> chars;

        /// Block type 5
        struct KerningPair
        {
            uint first;
            uint second;
            int16 amount;
        };
        vector<KerningPair> kernings;

        /// Read bmfont data into the object. Only supports the text file version.
        /// @param filepath path to the bmfont file to open
        /// @returns whether operation succeeded
        static bool fromFile(const string &filepath, BMFontData *data);

        static bool fromBuffer(const string &buffer, BMFontData *outData);

    };
}
