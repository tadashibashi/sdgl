#pragma once
#include "sdglib.h"

namespace sdgl {
#pragma pack(push, 1)
    class Color
    {
    public:
        /// Default color is white
        Color() : r(255), g(255), b(255), a(255) { }

        /// Set rgba values individually
        Color(const ubyte r, const ubyte g, const ubyte b, const ubyte a = 255) : r(r), g(g), b(b), a(a) { }

        /// Set from grayscale value
        explicit Color(const ubyte gray, const ubyte alpha = 255) : r(gray), g(gray), b(gray), a(alpha) { }

        ubyte r, g, b, a;

        /// Set rgba values
        Color &set(ubyte red, ubyte green, ubyte blue, ubyte alpha = 255);

        /// Set from grayscale value
        Color &setGrayScale(ubyte gray, ubyte alpha = 255);

        /// Set red channel
        Color &setR(ubyte value);
        /// Set green channel
        Color &setG(ubyte value);
        /// Set blue channel
        Color &setB(ubyte value);
        /// Set alpha channel
        Color &setA(ubyte value);

        /// Create color from an unsigned integer
        static Color fromRGBA(uint rgba);
        static Color fromARGB(uint argb);

        // Basic color constants
        // ----- grays -----
        static const Color White; ///< pure white (255, 255, 255)
        static const Color Gray;  ///< half way gray (128, 128, 128)
        static const Color Black; ///< pure black (0, 0, 0)

        // ----- primary colors -----
        static const Color Red;   ///< pure red (255, 0, 0)
        static const Color Green; ///< pure green (0, 255, 0)
        static const Color Blue;  ///< pure blue (0, 0, 255)

        // ----- secondary colors -----
        static const Color Cyan;    ///< (0, 255, 255)
        static const Color Magenta; ///< (255, 0, 255)
        static const Color Yellow;  ///< (255, 255, 0)

        // ----- tertiary colors -----
        static const Color Orange;      ///< (255, 128, 0)
        static const Color Chartreuse;  ///< (128, 255, 0)
        static const Color SpringGreen; ///< (0, 255, 128)
        static const Color Azure;       ///< (0, 128, 255)
        static const Color Violet;      ///< (128, 0, 255)
        static const Color Rose;        ///< (255, 0, 128)

        /// Convert to an unsigned 32-bit integer in RGBA channel order
        [[nodiscard]]
        uint toRGBA() const;
        /// Convert to an unsigned 32-bit integer in ARGB channel order
        [[nodiscard]]
        uint toARGB() const;
    };
#pragma pack(pop)
}
