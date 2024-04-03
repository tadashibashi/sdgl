#pragma once
#include "sdglib.h"

namespace sdgl {
#pragma pack(push, 1)
    class Color
    {
    public:
        /**
         * Default color is white
         */
        Color() : r(1.f), g(1.f), b(1.f), a(1.f) { }
        Color(float r, float g, float b, float a = 1.f) : r(r), g(g), b(b), a(a) { }
        explicit Color(float gray, float alpha = 1.f) : r(gray), g(gray), b(gray), a(alpha) { }

        float r, g, b, a;

        Color &set(float r, float g, float b, float a = 1.f);
        Color &setR(float value);
        Color &setG(float value);
        Color &setB(float value);
        Color &setA(float value);

        static Color fromRGBA(ubyte r, ubyte g, ubyte b, ubyte a = 0xFFu);
        static Color fromRGBA(uint rgba);
        static Color fromARGB(uint argb);

        // ----- grays -----
        static const Color White;
        static const Color Gray;
        static const Color Black;

        // ----- primary colors -----
        static const Color Red;
        static const Color Green;
        static const Color Blue;

        // ----- secondary colors -----
        static const Color Cyan;
        static const Color Magenta;
        static const Color Yellow;

        // ----- tertiary colors -----
        static const Color Orange;
        static const Color Chartreuse;
        static const Color SpringGreen;
        static const Color Azure;
        static const Color Violet;
        static const Color Rose;

        [[nodiscard]]
        uint toRGBA() const;
        [[nodiscard]]
        uint toARGB() const;
    };
#pragma pack(pop)
}
