#pragma once
#include "../sdgl_traits.h"
#include "Vector2.h"

namespace sdgl {
    template <Arithmetic T>
    class Rectangle_ {
    public:
        T x, y, w, h;

        constexpr Rectangle_(): x(0), y(0), w(0), h(0) { }
        constexpr Rectangle_(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) { }

        [[nodiscard]]
        float area() const { return w * h; }
        [[nodiscard]]
        Vector2_<T> location() const { return Vector2_<T>(x, y); }

        [[nodiscard]]
        Vector2_<T> size() const { return Vector2_<T>(w, h); }

        [[nodiscard]]
        T top() const { return y; }
        Rectangle_ &top(T value) { y = value; return *this; }

        [[nodiscard]]
        T left() const { return x; }
        Rectangle_ &left(T value) { x = value; return *this; }

        [[nodiscard]]
        T right() const { return x + w; }
        Rectangle_ &right(T value)
        {
            if (value < x)
                x = value;
            w = value - x;
            return *this;
        }

        [[nodiscard]]
        T bottom() const { return y + h; }
        Rectangle_ &bottom(T value)
        {
            if (value < y)
                y = value;
            h = value - y;
            return *this;
        }

        [[nodiscard]]
        Vector2_<T> topleft() const { return Vector2_<T>(left(), top()); }
        Rectangle_ &topleft(T x, T y) { return top(x).left(y); }
        Rectangle_ &topleft(Vector2_<T> value) { return top(value.x).left(value.y); }

        [[nodiscard]]
        Vector2_<T> topright() const { return Vector2_<T>(right(), top()); }
        Rectangle_ &topright(T x, T y) { return top(x).right(y); }
        Rectangle_ &topright(Vector2_<T> value) { return top(value.x).right(value.y); }

        [[nodiscard]]
        Vector2_<T> bottomleft() const { return Vector2_<T>(left(), bottom()); }
        Rectangle_ &bottomleft(T x, T y) { return bottom(x).left(y); }
        Rectangle_ &bottomleft(Vector2_<T> value) { return bottom(value.x).left(value.y); }

        [[nodiscard]]
        Vector2_<T> bottomright() const { return Vector2_<T>(right(), bottom()); }
        Rectangle_ &bottomright(T x, T y) { return bottom(x).right(y); }
        Rectangle_ &bottomright(Vector2_<T> value) { return bottom(value.x).right(value.y); }

        [[nodiscard]]
        bool operator==(const Rectangle_ &other) const
        {
            return x == other.x && y == other.y && w == other.w && h == other.h;
        }

        [[nodiscard]]
        bool operator!=(const Rectangle_ &other) const
        {
            return !operator==(other);
        }
    };

    using Rectangle = Rectangle_<int>;
    using FRectangle = Rectangle_<float>;
}