#pragma once
#include <sdgl/math/Rectangle.h>
#include <sdgl/math/Vector2.h>

namespace sdgl {
    class Camera2D {
    public:
        /// Projects a 2D orthographic view where {0, 0} is in the top-left corner
        explicit Camera2D();
        ~Camera2D();

        [[nodiscard]]
        Rectangle getViewport() const;


        /// Update the viewport position and size within the window. This should be set once during initialization.
        /// @param value rectangle to set
        /// @returns a reference to this camera for convenient chaining
        Camera2D &setViewport(Rectangle value);


        /// Get the origin offset position (default: {0, 0})
        /// @param normalized When `true`: returns normalized view coordinates where: {0, 0} is the top-left, and {1, 1}
        ///                   is the bottom right of the view
        ///                   When `false`: returns view coordinates in window pixels where: {0, 0} is the top-left, and
        ///                   {`viewport.w`, `viewport.y`} is the bottom right of the view
        /// Optionally set the `normalized` parameter to false to receive origin offset in view's window pixels.
        [[nodiscard]]
        Vector2 getOrigin(bool normalized=true) const;

        /// Set the origin offset position
        Camera2D &setOrigin(Vector2 value, bool normalized=true);

        /// Get the zoom scale for both x & y coordinates
        [[nodiscard]]
        Vector2 getScale() const;
        /// Set the zoom scale for both x & y coordinates
        Camera2D &setScale(Vector2 value);

        /// Get Z rotation in degrees
        [[nodiscard]]
        float getRotation() const;
        /// Set Z rotation in degrees
        Camera2D &setRotation(float value);

        /// Get the camera position in world units; the camera is like an actor in the world
        [[nodiscard]]
        Vector2 getPosition() const;
        Camera2D &setPosition(Vector2 position);

        /// Length is 16 (it's a 4x4 matrix), size should be `16 * sizeof(float)`
        [[nodiscard]]
        const float *getMatrix() const;

        /// Convert a view position to world position.
        /// @note view position units are in window pixels where {0, 0} is the top-left corner of the viewport. This may
        /// be offset by setting the camera's viewport to another position, whereby this function will calculate any
        /// transformations.
        [[nodiscard]]
        Vector2 viewToWorld(Vector2 viewCoords) const;

        /// Convert a position in world space to view position.
        /// @note world position units are in pixels where {0, 0} is the top-left corner. This may be scaled, translated,
        /// and rotated by the camera, whereby this function will calculate these transformations. When scale is {1, 1}
        /// and position {0, 0}, and rotation 0, world pixels will align with window pixels.
        /// View position units are in window pixels where {0, 0} is the top-left corner of the view.
        [[nodiscard]]
        Vector2 worldToView(Vector2 worldCoords) const;

        /// FIXME: doesn't take rotation into account; it probably will need to use a point for each corner to
        /// represent non-axis aligned rect
        [[nodiscard]]
        FRectangle getWorldBounds() const;
    private:
        struct Impl;
        Impl *m;
    };
}
