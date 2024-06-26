/**
 * Contains shape-related functionality
 */
#pragma once
#include <sdgl/sdgl_traits.h>
#include "Vector2.h"
#include "Rectangle.h"
#include "Circle.h"

namespace sdgl::mathf {
    template <Arithmetic T, Arithmetic U>
    [[nodiscard]]
    bool intersects(Vec2<T> a, Vec2<U> b) // rounds numbers for pixel-accuracy
    {
        return static_cast<int>(a.x + .5f) == static_cast<int>(b.x + .5f) &&
            static_cast<int>(a.y + .5f) == static_cast<int>(b.y + .5f);
    }

    template <Arithmetic T, Arithmetic U>
    [[nodiscard]]
    bool intersects(Rect<T> a, Rect<U> b)
    {
        return !(a.area() == 0 || b.area() == 0 ||
            a.right() < b.left() || a.bottom() < b.top() ||
            a.left() > b.right() || a.top() > b.bottom());
    }

    template <Arithmetic T, Arithmetic U>
    [[nodiscard]]
    bool intersects(Rect<T> rect, Vec2<U> point)
    {
        return !(rect.area() == 0 ||
            point.x < rect.left() || point.x > rect.right() ||
            point.y < rect.top() || point.y > rect.bottom());
    }

    template <Arithmetic T, Arithmetic U>
    [[nodiscard]]
    bool intersects(Vec2<U> point, Rect<T> rect)
    {
        return intersects(rect, point);
    }

    template <Arithmetic T>
    [[nodiscard]]
    bool intersects(Circle circ, Vec2<T> point)
    {
        return distance(circ.x, circ.y, point.x, point.y) < circ.radius;
    }

    template <Arithmetic T>
    [[nodiscard]]
    bool intersects(Vec2<T> point, Circle circ)
    {
        return intersects(circ, point);
    }

    [[nodiscard]]
    inline bool intersects(Circle a, Circle b)
    {
        const auto d = distance(a.x, a.y, b.x, b.y);
        return d < a.radius + b.radius;
    }

    template <Arithmetic T>
    [[nodiscard]]
    bool intersects(Circle circle, Rect<T> rect)
    {
        return intersects(Vector2(circle.x, circle.y), rect) ||
            intersects(circle, rect.topleft()) ||
            intersects(circle, rect.topright()) ||
            intersects(circle, rect.bottomleft()) ||
            intersects(circle, rect.bottomright());
    }

    template <Arithmetic T>
    [[nodiscard]]
    bool intersects(Rect<T> rect, Circle circle)
    {
        return intersects(circle, rect);
    }

    /// Get the resulting intersection of two rectangles
    template<Arithmetic T>
    Rect<T> intersection(const Rect<T> &a, const Rect<T> &b)
    {
        auto x = std::max(a.left(), b.left());
        auto y = std::max(a.top(), b.top());
        auto right = std::min(a.right(), b.right());
        auto bottom = std::min(a.bottom(), b.bottom());

        return {x, y, std::max(right - x, 0), std::max(bottom - y, 0)};
    }

    /// @return distance between two vectors
    template <Arithmetic U, Arithmetic V>
    [[nodiscard]]
    float distance(Vec2<U> a, Vec2<V> b)
    {
        return distance(a.x, a.y, b.x, b.y);
    }

    [[nodiscard]]
    inline Vector2 trajectory(const float angle, const float distance)
    {
        Vector2 res{};
        trajectory(angle, distance, &res.x, &res.y);
        return res;
    }

    /// Get the angle between two points
    template <Arithmetic T, Arithmetic U>
    [[nodiscard]]
    float pointAngle(Vec2<T> start, Vec2<U> end)
    {
        return pointAngle(end.x - start.x, end.y - start.y);
    }

    /// Rotate a Vec2 around {0, 0}
    template<Arithmetic T>
    [[nodiscard]]
    Vector2 rotate(Vec2<T> vec, const float angle)
    {
        Vector2 result;
        rotate(vec.x, vec.y, angle, &result.x, &result.y);
        return result;
    }
}
