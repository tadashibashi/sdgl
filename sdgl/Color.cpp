#include "Color.h"

#include "math/mathf.h"

namespace sdgl {
    Color & Color::set(float r, float g, float b, float a)
    {
        this->r = mathf::clamp(r, 0, 1.f);
        this->g = mathf::clamp(g, 0, 1.f);
        this->b = mathf::clamp(b, 0, 1.f);
        this->a = mathf::clamp(a, 0, 1.f);

        return *this;
    }

    Color &Color::setR(const float value)
    {
        r = mathf::clamp(value, 0.f, 1.f);
        return *this;
    }

    Color &Color::setG(const float value)
    {
        g = mathf::clamp(value, 0.f, 1.f);
        return *this;
    }

    Color &Color::setB(const float value)
    {
        b = mathf::clamp(value, 0.f, 1.f);
        return *this;
    }

    Color &Color::setA(const float value)
    {
        a = mathf::clamp(value, 0.f, 1.f);
        return *this;
    }

    Color Color::fromRGBA(const ubyte r, const ubyte g, const ubyte b, const ubyte a)
    {
        return {
            static_cast<float>(r) / 255.f,
            static_cast<float>(g) / 255.f,
            static_cast<float>(b) / 255.f,
            static_cast<float>(a) / 255.f
        };
    }

    Color Color::fromRGBA(uint rgba)
    {
        const auto a = static_cast<float>(rgba % 0xFF) / 255.f;
        rgba >>= 8;
        const auto b = static_cast<float>(rgba % 0xFF) / 255.f;
        rgba >>= 8;
        const auto g = static_cast<float>(rgba % 0xFF) / 255.f;
        rgba >>= 8;
        const auto r = static_cast<float>(rgba % 0xFF) / 255.f;

        return {r, g, b, a};
    }

    Color Color::fromARGB(uint argb)
    {
        const auto b = static_cast<float>(argb % 0xFF) / 255.f;
        argb >>= 8;
        const auto g = static_cast<float>(argb % 0xFF) / 255.f;
        argb >>= 8;
        const auto r = static_cast<float>(argb % 0xFF) / 255.f;
        argb >>= 8;
        const auto a = static_cast<float>(argb % 0xFF) / 255.f;

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
            static_cast<uint>(std::round(a * 255.f)) << 24 |
            static_cast<uint>(std::round(r * 255.f)) << 16 |
            static_cast<uint>(std::round(g * 255.f)) << 8  |
            static_cast<uint>(std::round(b * 255.f));
    }

    // ----- grays ------
    const Color Color::White = Color(1.0f, 1.0f, 1.0f);
    const Color Color::Gray = Color(.5f, .5f, .5f);
    const Color Color::Black = Color(0, 0, 0);

    // ----- primary colors -----
    const Color Color::Red = Color(1.f, 0, 0);
    const Color Color::Green = Color(0, 1.f, 0);
    const Color Color::Blue = Color(0, 0, 1.f);

    // ----- secondary colors -----
    const Color Color::Cyan = Color(0, 1.f, 1.f);
    const Color Color::Magenta = Color(1.f, 0, 1.f);
    const Color Color::Yellow = Color(1.f, 1.f, 0);

    // ----- tertiary colors -----
    const Color Color::Orange = Color(1.f, .5f, 0);
    const Color Color::Chartreuse = Color(.5f, 1.f, 0);
    const Color Color::SpringGreen = Color(0, 1.f, .5f);
    const Color Color::Azure = Color(0, .5f, 1.f);
    const Color Color::Violet = Color(.5f, 0, 1.f);
    const Color Color::Rose = Color(1.f, 0, .5f);
}
