/**
 * Contains shape-related functionality
 */
#pragma once
#include <sdgl/sdgl_traits.h>
#include "Vector2.h"
#include "Rectangle.h"

namespace sdgl::mathf {
    template <Arithmetic T, Arithmetic U>
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

    /**
     * @return distance between two vectors
     */
    template <Arithmetic U, Arithmetic V>
    float distance(Vec2<U> a, Vec2<V> b)
    {
        return distance(a.x, a.y, b.x, b.y);
    }

    inline Vec2<float> trajectory(const float angle, const float distance)
    {
        Vec2<float> res{};
        trajectory(angle, distance, &res.x, &res.y);
        return res;
    }

    template <Arithmetic T, Arithmetic U>
    float pointAngle(Vec2<T> a, Vec2<U> b)
    {
        return pointAngle(b.x - a.x, b.y - a.y);
    }

    template<Arithmetic T>
    Vector2 rotate(Vec2<T> vec, const float angle)
    {
        Vector2 result;
        rotate(vec.x, vec.y, angle, &result.x, &result.y);
        return result;
    }
}
