/**
 * Algorithms from https://easings.net/, usage under GNU General Public License v3.0
 */
#include "TweenFunction.h"

#include "mathf.h"

#include <cmath>

namespace sdgl::tweenf {
    float linear(const float x)
    {
        return x;
    }

    float easeInSine(const float x)
    {
        return 1.f - std::cos((x * mathf::Pi) * .5f);
    }

    float easeOutSine(const float x)
    {
        return std::sin((x * mathf::Pi) * .5f);
    }

    float easeInOutSine(const float x)
    {
        return -(std::cos(mathf::Pi * x) - 1.f) * .5f;
    }

    float easeInQuad(const float x)
    {
        return x * x;
    }

    float easeOutQuad(const float x)
    {
        return 1 - (1 - x) * (1 - x);
    }

    float easeInOutQuad(const float x)
    {
        return x < 0.5f ?
            2.f * x * x :
            1.f - std::pow(-2.f * x + 2.f, 2.f) * .5f;
    }

    float easeInCubic(const float x)
    {
        return x * x * x;
    }

    float easeOutCubic(const float x)
    {
        return 1.f - std::pow(1.f - x, 3.f);
    }

    float easeInOutCubic(const float x)
    {
        return x < 0.5f ?
            4.f * x * x * x :
            1.f - std::pow(-2.f * x + 2.f, 3.f) * .5f;
    }

    float easeInQuint(const float x)
    {
        return x * x * x * x * x;
    }

    float easeOutQuint(const float x)
    {
        return 1.f - std::pow(1.f - x, 5.f);
    }

    float easeInOutQuint(const float x)
    {
        return x < 0.5f ?
            16.f * x * x * x * x * x :
            1.f - std::pow(-2.f * x + 2.f, 5.f) * .5f;
    }

    float easeInExpo(const float x)
    {
        return x == 0 ?
            0 :
            std::pow(2.f, 10.f * x - 10.f);
    }

    float easeOutExpo(const float x)
    {
        return x == 1.f ?
            1.f :
            1.f - std::pow(2.f, -10.f * x);
    }

    float easeInOutExpo(const float x)
    {
        return x == 0 ? 0
            : x == 1.f ? 1.f
            : x < 0.5f ? std::pow(2.f, 20.f * x - 10.f) * .5f
            : (2.f - std::pow(2.f, -20.f * x + 10.f)) * .5f;
    }

    float easeInCirc(const float x)
    {
        return 1.f - std::sqrt(1.f - std::pow(x, 2.f));
    }

    float easeOutCirc(const float x)
    {
        return std::sqrt(1.f - std::pow(x - 1.f, 2.f));
    }

    float easeInOutCirc(const float x)
    {
        return x < 0.5f ?
            (1.f - std::sqrt(1.f - std::pow(2.f * x, 2.f))) * .5f :
            (std::sqrt(1.f - std::pow(-2.f * x + 2.f, 2.f)) + 1.f) * .5f;
    }

    float easeInBack(const float x)
    {
        static constexpr float c1 = 1.70158f;
        static constexpr float c3 = c1 + 1.f;

        return c3 * x * x * x - c1 * x * x;
    }

    float easeOutBack(const float x)
    {
        static constexpr float c1 = 1.70158f;
        static constexpr float c3 = c1 + 1.f;

        return 1.f + c3 * std::pow(x - 1.f, 3.f) + c1 * std::pow(x - 1.f, 2.f);
    }

    float easeInOutBack(const float x)
    {
        static constexpr float c1 = 1.70158f;
        static constexpr float c2 = c1 * 1.525f;

        return x < 0.5f
          ? (std::pow(2.f * x, 2.f) * ((c2 + 1.f) * 2.f * x - c2)) * .5f
          : (std::pow(2.f * x - 2.f, 2.f) * ((c2 + 1.f) * (x * 2.f - 2.f) + c2) + 2.f) * .5f;
    }

    float easeInElastic(const float x)
    {
        static constexpr float c4 = (2.f * mathf::Pi) / 3.f;

        return x == 0
          ? 0
          : x == 1.f
          ? 1.f
          : -std::pow(2.f, 10.f * x - 10.f) * std::sin((x * 10.f - 10.75f) * c4);
    }

    float easeOutElastic(const float x)
    {
        static constexpr float c4 = (2.f * mathf::Pi) / 3.f;

        return x == 0
          ? 0
          : x == 1.f
          ? 1.f
          : std::pow(2.f, -10.f * x) * std::sin((x * 10.f - 0.75f) * c4) + 1.f;
    }

    float easeInOutElastic(const float x)
    {
        static constexpr float c5 = (2.f * mathf::Pi) / 4.5f;

        return x == 0
          ? 0
          : x == 1.f
          ? 1.f
          : x < 0.5f
          ? -(std::pow(2.f, 20.f * x - 10.f) * std::sin((20.f * x - 11.125f) * c5)) * .5f
          : (std::pow(2.f, -20.f * x + 10.f) * std::sin((20.f * x - 11.125f) * c5)) * .5f + 1.f;
    }

    float easeInBounce(const float x)
    {
        return 1.f - easeOutBounce(1.f - x);
    }

    float easeOutBounce(float x)
    {
        static constexpr float n1 = 7.5625f;
        static constexpr float d1 = 2.75f;

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

    float easeInOutBounce(float x)
    {
        return x < 0.5f ?
            (1.f - easeOutBounce(1.f - 2.f * x)) * .5f :
            (1.f + easeOutBounce(2.f * x - 1.f)) * .5f;
    }
}
