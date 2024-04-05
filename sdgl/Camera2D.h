#pragma once
#include <sdgl/math/Vector2.h>
#include <sdgl/math/Rectangle.h>

namespace sdgl {
    class Camera2D {
    public:
        explicit Camera2D();
        ~Camera2D();

        [[nodiscard]]
        Rectangle getViewport() const;

        /**
         * Update the viewport once
         * @param value rectangle to set
         * @return reference to this camera for convenient chaining
         */
        Camera2D &setViewport(Rectangle value);

        /**
         * @return normalized origin position where {0, 0} is the top-left, and {1, 1} is the bottom right
         * of the screen (default: {0, 0}). Note: this only affects the output if a viewport is set
         */
        [[nodiscard]]
        Vector2 getOrigin() const;
        Camera2D &setOrigin(Vector2 value);


        [[nodiscard]]
        Vector2 getScale() const;
        Camera2D &setScale(Vector2 value);

        [[nodiscard]]
        float getRotation() const;
        Camera2D &setRotation(float value);

        [[nodiscard]]
        Vector2 getPosition() const;
        Camera2D &setPosition(Vector2 position);

        /** Length is 16 as a 4x4 matrix, size should be `16 * sizeof(float)` */
        [[nodiscard]]
        const float *getMatrix() const;

        [[nodiscard]]
        Vector2 viewToWorld(Vector2 viewCoords) const;

        [[nodiscard]]
        Vector2 worldToView(Vector2 worldCoords) const;

        [[nodiscard]]
        FRectangle getWorldBounds() const;
    private:
        struct Impl;
        Impl *m;
    };
}
