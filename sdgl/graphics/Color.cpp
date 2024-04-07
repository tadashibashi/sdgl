#include "Color.h"

#include "../math/mathf.h"

namespace sdgl {
    Color & Color::set(ubyte red, ubyte green, ubyte blue, ubyte alpha)
    {
        this->r = red;
        this->g = green;
        this->b = blue;
        this->a = alpha;

        return *this;
    }

    Color & Color::setGrayScale(ubyte gray, ubyte alpha)
    {
        r = gray;
        g = gray;
        b = gray;
        a = alpha;
        return *this;
    }

    Color &Color::setR(const ubyte value)
    {
        r = value;
        return *this;
    }

    Color &Color::setG(const ubyte value)
    {
        g = value;
        return *this;
    }

    Color &Color::setB(const ubyte value)
    {
        b = value;
        return *this;
    }

    Color &Color::setA(const ubyte value)
    {
        a = value;
        return *this;
    }

    Color Color::fromRGBA(uint rgba)
    {
        const ubyte a = rgba % 0xFF;
        rgba >>= 8;
        const ubyte b = rgba % 0xFF;
        rgba >>= 8;
        const ubyte g = rgba % 0xFF;
        rgba >>= 8;
        const ubyte r = rgba % 0xFF;

        return {r, g, b, a};
    }

    Color Color::fromARGB(uint argb)
    {
        const ubyte b = argb % 0xFF;
        argb >>= 8;
        const ubyte g = argb % 0xFF;
        argb >>= 8;
        const ubyte r = argb % 0xFF;
        argb >>= 8;
        const ubyte a = argb % 0xFF;

        return {r, g, b, a};
    }

    uint Color::toRGBA() const
    {
        return
            static_cast<uint>(std::round(r * 255.f)) << 24 |
            static_cast<uint>(std::round(g * 255.f)) << 16 |
            static_cast<uint>(std::round(b * 255.f)) << 8  |
            static_cast<uint>(std::round(a * 255.f));
    }

    uint Color::toARGB() const
    {
        return
            a << 24 |
            r << 16 |
            g << 8  |
            b;
    }

    // ----- grays ------
    const Color Color::White = Color(255, 255, 255);
    const Color Color::Gray = Color(128, 128, 128);
    const Color Color::Black = Color(0, 0, 0);

    // ----- primary colors -----
    const Color Color::Red = Color(255, 0, 0);
    const Color Color::Green = Color(0, 255, 0);
    const Color Color::Blue = Color(0, 0, 255);

    // ----- secondary colors -----
    const Color Color::Cyan = Color(0, 255, 255);
    const Color Color::Magenta = Color(255, 0, 255);
    const Color Color::Yellow = Color(255, 255, 0);

    // ----- tertiary colors -----
    const Color Color::Orange = Color(255, 128, 0);
    const Color Color::Chartreuse = Color(128, 255, 0);
    const Color Color::SpringGreen = Color(0, 255, 128);
    const Color Color::Azure = Color(0, 128, 255);
    const Color Color::Violet = Color(128, 0, 255);
    const Color Color::Rose = Color(255, 0, 128);
}
