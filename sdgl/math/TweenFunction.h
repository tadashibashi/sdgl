#pragma once

namespace sdgl::tweenf {
    float linear(float x);

    float easeInSine(float x);
    float easeOutSine(float x);
    float easeInOutSine(float x);

    float easeInQuad(float x);
    float easeOutQuad(float x);
    float easeInOutQuad(float x);

    float easeInCubic(float x);
    float easeOutCubic(float x);
    float easeInOutCubic(float x);

    float easeInQuint(float x);
    float easeOutQuint(float x);
    float easeInOutQuint(float x);

    float easeInExpo(float x);
    float easeOutExpo(float x);
    float easeInOutExpo(float x);

    float easeInCirc(float x);
    float easeOutCirc(float x);
    float easeInOutCirc(float x);

    float easeInBack(float x);
    float easeOutBack(float x);
    float easeInOutBack(float x);

    float easeInElastic(float x);
    float easeOutElastic(float x);
    float easeInOutElastic(float x);

    float easeInBounce(float x);
    float easeOutBounce(float x);
    float easeInOutBounce(float x);
}