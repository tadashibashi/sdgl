#pragma once

#include <sdgl/math/mathf.h>
#include <sdgl/sdgl_traits.h>

namespace sdgl {

#pragma pack(push, 1)
    template <Arithmetic T>
    class Vec2 {
    public:
        T x, y;
        constexpr Vec2() : x(0), y(0) { }
        constexpr Vec2(T x, T y) : x(x), y(y) { }

        static const Vec2<T> Zero;
        static const Vec2<T> One;

        template <Arithmetic U>
        [[nodiscard]]
        bool operator==(Vec2<U> other) const { return x == other.x && y == other.y;  }

        template <Arithmetic U>
        [[nodiscard]]
        bool operator!=(Vec2<U> other) const { return x != other.x || y != other.y;  }

        template <Arithmetic U>
        Vec2 operator+(Vec2<U> other) const
        {
            return Vec2<T>(x + other.x, y + other.y);
        }

        template <Arithmetic U>
        Vec2 &operator+=(Vec2<U> other)
        {
            x += other.x; y += other.y;
            return *this;
        }

        template <Arithmetic U>
        Vec2 operator-(Vec2<U> other) const
        {
            return Vec2<T>(x - other.x, y - other.y);
        }

        template <Arithmetic U>
        Vec2 &operator-=(Vec2<U> other)
        {
            x -= other.x; y -= other.y;
            return *this;
        }

        template <Arithmetic U>
        Vec2 operator*(Vec2<U> other) const
        {
            return Vec2<T>(x * other.x, y * other.y);
        }

        template <Arithmetic U>
        Vec2 &operator*=(Vec2<U> other)
        {
            x *= other.x; y *= other.y;
            return *this;
        }

        template <Arithmetic U>
        Vec2 operator/(Vec2<U> other) const
        {
            return Vec2<T>(x / other.x, y / other.y);
        }

        template <Arithmetic U>
        Vec2 &operator/=(Vec2<U> other)
        {
            x /= other.x; y /= other.y;
            return *this;
        }

        template <Arithmetic U>
        Vec2 operator*(U scalar) const
        {
            return Vec2<T>(x * scalar, y * scalar);
        }

        template <Arithmetic U>
        Vec2 &operator*=(U scalar)
        {
            x *= scalar; y *= scalar;
            return *this;
        }

        template <Arithmetic U>
        Vec2 operator/(U scalar) const
        {
            return Vec2<T>(x / scalar, y / scalar);
        }

        template <Arithmetic U>
        Vec2 &operator/=(U scalar)
        {
            x /= scalar; y /= scalar;
            return *this;
        }

        Vec2 operator -()
        {
            return Vec2(-x, -y);
        }

        Vec2 operator +()
        {
            return Vec2(x, y);
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
        Vec2 normal() const
        {
            auto l = length();
            return (l == 0) ?  Zero : Vec2(x / l, y / l);
        }

        template <Arithmetic U>
        [[nodiscard]]
        float angleTo(Vec2<U> other) const
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
        explicit operator Vec2<U>() const
        {
            return Vec2<U>(static_cast<U>(x), static_cast<U>(y));
        }
    };
#pragma pack(pop)

    template<Arithmetic T>
    const Vec2<T> Vec2<T>::Zero = Vec2(0, 0);

    template<Arithmetic T>
    const Vec2<T> Vec2<T>::One = Vec2(1, 1);

    using Point = Vec2<int>;
    using Vector2 = Vec2<float>;
}

#include <sdgl/format.h>
template <typename T>
struct formatter<sdgl::Vec2<T>>
{
    template <class FormatContext>
    constexpr auto parse(FormatContext& ctx) {
        return ctx.begin();
    }

    template <class FormatContext>
    auto format(const sdgl::Vec2<T>& obj, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "({}, {})", obj.x, obj.y);
    }
};
