#pragma once
#include <sdgl/math/Vector2.h>

namespace sdgl {
    class Circle {
    public:
        Circle() : x(), y(), radius() { }
        Circle(float x, float y, float radius) : x(x), y(y), radius(radius) { }

        float x, y, radius;

        [[nodiscard]]
        float diameter() const { return radius * 2; }

        [[nodiscard]]
        float area() const;

        [[nodiscard]]
        float circumference() const;

        [[nodiscard]]
        Vector2 position() const { return {x, y}; }

        void position(float x, float y)
        {
            this->x = x;
            this->y = y;
        }
    };
}
