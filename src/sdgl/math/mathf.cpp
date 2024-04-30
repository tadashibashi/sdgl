#include "mathf.h"
#include <cmath>

// Ensure these definitions exist

#ifndef M_PI_4
#define M_PI_4 0.785398163397448309615660845819875721
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923132169163975144
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif


namespace sdgl::mathf {
    const float FourthPi = M_PI_4;
    const float HalfPi = M_PI_2;
    const float Pi = M_PI;
    const float TwoPi = M_PI * 2.f;

    float distance(const float x1, const float y1, const float x2, const float y2)
    {
        const auto x = x2 - x1;
        const auto y = y2 - y1;
        return std::sqrtf(x*x + y*y);
    }

    float modf(const float x, const float m)
    {
        return std::fmodf(std::fmodf(x, m) + m, m);
    }

    float clamp(const float x, const float min, const float max)
    {
        return x < min ? min :
            x > max ? max :
            x;
    }

    int clampi(const int x, const int min, int max)
    {
        --max;
        return x < min ? min :
            x > max ? max :
            x;
    }

    float wrap(const float x, const float min, const float max)
    {
        return modf(x - min, max - min) + min;
    }

    float toRadians(const float degrees)
    {
        constexpr static float k = M_PI / 180.f;
        return degrees * k;
    }

    float toDegrees(const float radians)
    {
        constexpr static float k = 180.f / M_PI;
        return radians * k;
    }

    float pointAngle(const float x, const float y)
    {
        return std::fmodf((std::atan2f(x, y) + TwoPi), TwoPi);
    }

    void trajectory(const float angle, const float distance, float *x, float *y)
    {
        if (x)
            *x = std::sinf(angle) * distance;
        if (y)
            *y = std::cosf(angle) * distance;
    }

    void rotate(const float x, const float y, const float angle, float *resultX, float *resultY)
    {
        const auto sinAngle = std::sin(angle);
        const auto cosAngle = std::cos(angle);
        if (resultX)
        {
            *resultX = x * cosAngle - y * sinAngle;
        }

        if (resultY)
        {
            *resultY = x * sinAngle + y * cosAngle;
        }
    }
}
