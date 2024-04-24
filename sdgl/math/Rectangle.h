#pragma once
#include <sdgl/sdgl_traits.h>
#include "Vector2.h"

namespace sdgl {
    template <Arithmetic T>
    class Rect {
    public:
        T x, y, w, h;

        constexpr Rect(): x(0), y(0), w(0), h(0) { }
        constexpr Rect(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) { }

        [[nodiscard]]
        float area() const { return w * h; }
        [[nodiscard]]
        Vec2<T> location() const { return Vec2<T>(x, y); }

        [[nodiscard]]
        Vec2<T> size() const { return Vec2<T>(w, h); }

        [[nodiscard]]
        T top() const { return y; }
        Rect &top(T value) { y = value; return *this; }

        [[nodiscard]]
        T left() const { return x; }
        Rect &left(T value) { x = value; return *this; }

        [[nodiscard]]
        T right() const { return x + w; }
        Rect &right(T value)
        {
            if (value < x)
                x = value;
            w = value - x;
            return *this;
        }

        [[nodiscard]]
        T bottom() const { return y + h; }
        Rect &bottom(T value)
        {
            if (value < y)
                y = value;
            h = value - y;
            return *this;
        }

        [[nodiscard]]
        Vec2<T> topleft() const { return Vec2<T>(left(), top()); }
        Rect &topleft(T x, T y) { return top(x).left(y); }
        Rect &topleft(Vec2<T> value) { return top(value.x).left(value.y); }

        [[nodiscard]]
        Vec2<T> topright() const { return Vec2<T>(right(), top()); }
        Rect &topright(T x, T y) { return top(x).right(y); }
        Rect &topright(Vec2<T> value) { return top(value.x).right(value.y); }

        [[nodiscard]]
        Vec2<T> bottomleft() const { return Vec2<T>(left(), bottom()); }
        Rect &bottomleft(T x, T y) { return bottom(x).left(y); }
        Rect &bottomleft(Vec2<T> value) { return bottom(value.x).left(value.y); }

        [[nodiscard]]
        Vec2<T> bottomright() const { return Vec2<T>(right(), bottom()); }
        Rect &bottomright(T x, T y) { return bottom(x).right(y); }
        Rect &bottomright(Vec2<T> value) { return bottom(value.x).right(value.y); }

        [[nodiscard]]
        bool operator==(const Rect &other) const
        {
            return x == other.x && y == other.y && w == other.w && h == other.h;
        }

        [[nodiscard]]
        bool operator!=(const Rect &other) const
        {
            return !operator==(other);
        }

        template <Arithmetic U>
        Rect operator+(const Rect<U> &other) const
        {
            return {x + other.x, y + other.y, w + other.w, h + other.h};
        }

        template <Arithmetic U>
        Rect &operator+=(const Rect<U> &other)
        {
            x += other.x;
            y += other.y;
            w += other.w;
            h += other.h;
            return *this;
        }

        template <Arithmetic U>
        Rect operator-(const Rect<U> &other) const
        {
            return {x - other.x, y - other.y, w - other.w, h - other.h};
        }

        template <Arithmetic U>
        Rect &operator-=(const Rect<U> &other)
        {
            x -= other.x;
            y -= other.y;
            w -= other.w;
            h -= other.h;
            return *this;
        }


        template <Arithmetic U>
        [[nodiscard]] explicit
        operator Rect<U>() const
        {
            return Rect<U>(
                static_cast<U>(x),
                static_cast<U>(y),
                static_cast<U>(w),
                static_cast<U>(h)
            );
        }
    };

    using Rectangle = Rect<int>;
    using FRectangle = Rect<float>;
}

#include <sdgl/format.h>
template <typename T>
struct formatter<sdgl::Rect<T>>
{
    template <class FormatContext>
    constexpr auto parse(FormatContext& ctx) {
        return ctx.begin();
    }

    template <class FormatContext>
    auto format(const sdgl::Rect<T>& obj, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "({}, {}, {}, {})", obj.x, obj.y, obj.w, obj.h);
    }
};