#pragma once
#include <sdgl/math/mathf.h>
#include <sdgl/sdgl_traits.h>

namespace sdgl {

#pragma pack(push, 1)
    template <Arithmetic T>
    class Vector2_ {
    public:
        T x, y;
        constexpr Vector2_() : x(0), y(0) { }
        constexpr Vector2_(T x, T y) : x(x), y(y) { }

        static const Vector2_<T> Zero;
        static const Vector2_<T> One;

        template <Arithmetic U>
        [[nodiscard]]
        bool operator==(Vector2_<U> other) const { return x == other.x && y == other.y;  }

        template <Arithmetic U>
        [[nodiscard]]
        bool operator!=(Vector2_<U> other) const { return x != other.x || y != other.y;  }

        template <Arithmetic U>
        Vector2_ operator+(Vector2_<U> other) const
        {
            return _Vector2<T>(x + other.x, y + other.y);
        }

        template <Arithmetic U>
        Vector2_ &operator+=(Vector2_<U> other)
        {
            x += other.x; y += other.y;
            return *this;
        }

        template <Arithmetic U>
        Vector2_ operator-(Vector2_<U> other) const
        {
            return _Vector2<T>(x - other.x, y - other.y);
        }

        template <Arithmetic U>
        Vector2_ &operator-=(Vector2_<U> other)
        {
            x -= other.x; y -= other.y;
            return *this;
        }

        template <Arithmetic U>
        Vector2_ operator*(Vector2_<U> other) const
        {
            return _Vector2<T>(x * other.x, y * other.y);
        }

        template <Arithmetic U>
        Vector2_ &operator*=(Vector2_<U> other)
        {
            x *= other.x; y *= other.y;
            return *this;
        }

        template <Arithmetic U>
        Vector2_ operator/(Vector2_<U> other) const
        {
            return _Vector2<T>(x / other.x, y / other.y);
        }

        template <Arithmetic U>
        Vector2_ &operator/=(Vector2_<U> other)
        {
            x /= other.x; y /= other.y;
            return *this;
        }

        template <Arithmetic U>
        Vector2_ operator*(U scalar) const
        {
            return _Vector2<T>(x * scalar, y * scalar);
        }

        template <Arithmetic U>
        Vector2_ &operator*=(U scalar)
        {
            x *= scalar; y *= scalar;
            return *this;
        }

        template <Arithmetic U>
        Vector2_ operator/(U scalar) const
        {
            return _Vector2<T>(x / scalar, y / scalar);
        }

        template <Arithmetic U>
        Vector2_ &operator/=(U scalar)
        {
            x /= scalar; y /= scalar;
            return *this;
        }

        /**
         * @return distance of this vector from {0, 0}
         */
        [[nodiscard]]
        float length() const
        {
            return mathf::distance(0, 0, x, y);
        }

        /**
         * @return normalized copy of this vector - does not mutate
         * If length is zero, a zero vector will be returned
         */
        [[nodiscard]]
        Vector2_ normal() const
        {
            auto l = length();
            return (l == 0) ?  Zero : _Vector2(x / l, y / l);
        }

        template <Arithmetic U>
        [[nodiscard]]
        float angleTo(Vector2_<U> other) const
        {
            return mathf::pointAngle(other.x - x, other.y - y);
        }

        /**
         * Angle, in radians from {0, 0} to this vector
         */
        [[nodiscard]]
        float angle() const
        {
            return mathf::pointAngle(x, y);
        }

        template <Arithmetic U>
        explicit operator Vector2_<U>() const
        {
            return Vector2_<U>(static_cast<U>(x), static_cast<U>(y));
        }
    };
#pragma pack(pop)

    template<Arithmetic T>
    const Vector2_<T> Vector2_<T>::Zero = Vector2_(0, 0);

    template<Arithmetic T>
    const Vector2_<T> Vector2_<T>::One = Vector2_(1, 1);

    using Point = Vector2_<int>;
    using Vector2 = Vector2_<float>;
}
