#include "easings.h"

#include <sdgl/math/mathf.h>

#include <cmath>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"

namespace sdgl::easings {

    float linear(const float x)
    {
        return x;
    }

    float inSine(const float x)
    {
        return 1.f - std::cos((x * mathf::Pi) * .5f);
    }

    float outSine(const float x)
    {
        return std::sin((x * mathf::Pi) * .5f);
    }

    float inOutSine(const float x)
    {
        return -(std::cos(mathf::Pi * x) - 1.f) * .5f;
    }

    float inQuad(const float x)
    {
        return x * x;
    }

    float outQuad(const float x)
    {
        return 1.f - (1.f - x) * (1.f - x);
    }

    float inOutQuad(const float x)
    {
        return x < 0.5f ?
            2.f * x * x :
            1.f - std::pow(-2.f * x + 2.f, 2.f) * .5f;
    }

    float inCubic(const float x)
    {
        return x * x * x;
    }

    float outCubic(const float x)
    {
        return 1.f - std::pow(1.f - x, 3.f);
    }

    float inOutCubic(const float x)
    {
        return x < 0.5f ?
            4.f * x * x * x :
            1.f - std::pow(-2.f * x + 2.f, 3.f) * .5f;
    }

    float inQuint(const float x)
    {
        return x * x * x * x * x;
    }

    float outQuint(const float x)
    {
        return 1.f - std::pow(1.f - x, 5.f);
    }

    float inOutQuint(const float x)
    {
        return x < 0.5f ?
            16.f * x * x * x * x * x :
            1.f - std::pow(-2.f * x + 2.f, 5.f) * .5f;
    }

    float inExpo(const float x)
    {
        return x == 0 ?
            0 :
            std::pow(2.f, 10.f * x - 10.f);
    }

    float outExpo(const float x)
    {
        return x == 1.f ?
            1.f :
            1.f - std::pow(2.f, -10.f * x);
    }

    float inOutExpo(const float x)
    {
        return x == 0 ? 0
            : x == 1.f ? 1.f
            : x < 0.5f ? std::pow(2.f, 20.f * x - 10.f) * .5f
            : (2.f - std::pow(2.f, -20.f * x + 10.f)) * .5f;
    }

    float inCirc(const float x)
    {
        return 1.f - std::sqrt(1.f - std::pow(x, 2.f));
    }

    float outCirc(const float x)
    {
        return std::sqrt(1.f - std::pow(x - 1.f, 2.f));
    }

    float inOutCirc(const float x)
    {
        return x < .5f ?
            (1.f - std::sqrt(1.f - std::pow(2.f * x, 2.f))) * .5f :
            (std::sqrt(1.f - std::pow(-2.f * x + 2.f, 2.f)) + 1.f) * .5f;
    }

    float inBack(const float x)
    {
        static const float c1 = 1.70158f;
        static const float c3 = c1 + 1.f;

        return c3 * x * x * x - c1 * x * x;
    }

    float outBack(const float x)
    {
        static const float c1 = 1.70158f;
        static const float c3 = c1 + 1.f;

        return 1.f + c3 * std::pow(x - 1.f, 3.f) + c1 * std::pow(x - 1.f, 2.f);
    }

    float inOutBack(const float x)
    {
        static const float c1 = 1.70158f;
        static const float c2 = c1 * 1.525f;

        return x < 0.5f
          ? (std::pow(2.f * x, 2.f) * ((c2 + 1.f) * 2.f * x - c2)) * .5f
          : (std::pow(2.f * x - 2.f, 2.f) * ((c2 + 1.f) * (x * 2.f - 2.f) + c2) + 2.f) * .5f;
    }

    float inElastic(const float x)
    {
        static const float c4 = (2.f * mathf::Pi) / 3.f;

        return x == 0
          ? 0
          : x == 1.f
          ? 1.f
          : -std::pow(2.f, 10.f * x - 10.f) * std::sin((x * 10.f - 10.75f) * c4);
    }

    float outElastic(const float x)
    {
        static const float c4 = (2.f * mathf::Pi) / 3.f;

        return x == 0
          ? 0
          : x == 1.f
          ? 1.f
          : std::pow(2.f, -10.f * x) * std::sin((x * 10.f - 0.75f) * c4) + 1.f;
    }

    float inOutElastic(const float x)
    {
        static const float c5 = (2.f * mathf::Pi) / 4.5f;

        return x == 0
          ? 0
          : x == 1.f
          ? 1.f
          : x < 0.5f
          ? -(std::pow(2.f, 20.f * x - 10.f) * std::sin((20.f * x - 11.125f) * c5)) * .5f
          : (std::pow(2.f, -20.f * x + 10.f) * std::sin((20.f * x - 11.125f) * c5)) * .5f + 1.f;
    }

    float inBounce(const float x)
    {
        return 1.f - outBounce(1.f - x);
    }

    float outBounce(float x)
    {
        static const float n1 = 7.5625f;
        static const float d1 = 2.75f;

        if (x < 1.f / d1) {
            return n1 * x * x;
        }

        if (x < 2.f / d1) {
            return n1 * (x -= 1.5f / d1) * x + 0.75f;
        }

        if (x < 2.5 / d1) {
            return n1 * (x -= 2.25f / d1) * x + 0.9375f;
        }

        return n1 * (x -= 2.625f / d1) * x + 0.984375f;
    }

    float inOutBounce(float x)
    {
        return x < 0.5f ?
            (1.f - outBounce(1.f - 2.f * x)) * .5f :
            (1.f + outBounce(2.f * x - 1.f)) * .5f;
    }
}
#pragma clang diagnostic pop
