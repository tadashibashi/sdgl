#pragma once

namespace sdgl::mathf
{
    extern const float FourthPi;
    extern const float HalfPi;
    extern const float Pi;
    extern const float TwoPi;

    /**
     * Get the distance between two points
     */
    float distance(float x1, float y1, float x2, float y2);

    /**
     * fmodf that continues cyclic pattern when negative (doesn't flip when negative)
     */
    float modf(float x, float m);

    /**
     * Inclusively clamp `x` between `min` and `max`
     */
    float clamp(float x, float min, float max);

    /**
     * Clamp `x` between `min` (inclusive), and `max` (exclusive)
     */
    int clampi(int x, int min, int max);

    /**
     * Wrap values across `min` (inclusive) and `max` (exclusive)
     * e.g. Behaves as a modulus but without flipping when value is negative
     */
    float wrap(float x, float min, float max);

    float toRadians(float degrees);

    float toDegrees(float radians);

    /**
     * Get the angle, in radians from {0, 0} to a given point
     */
    float pointAngle(float x, float y);

    /**
     * Get the trajected x and y coordinates for traveling `distance` at `angle` (in radians) (from {0, 0})
     */
    void trajectory(float angle, float distance, float *x, float *y);

    /**
     * Get the result of a rotation of a vector about {0, 0}
     * @param x - x position to rotate
     * @param y - y position to rotate
     * @param angle - angle in radians clockwise
     * @param resultX - resulting x value
     * @param resultY - resulting y vaue
     */
    void rotate(float x, float y, float angle, float *resultX, float *resultY);
}
