#pragma once

namespace sdgl::easings {
    float linear(float x);

    float inSine(float x);
    float outSine(float x);
    float inOutSine(float x);

    float inQuad(float x);
    float outQuad(float x);
    float inOutQuad(float x);

    float inCubic(float x);
    float outCubic(float x);
    float inOutCubic(float x);

    float inQuint(float x);
    float outQuint(float x);
    float inOutQuint(float x);

    float inExpo(float x);
    float outExpo(float x);
    float inOutExpo(float x);

    float inCirc(float x);
    float outCirc(float x);
    float inOutCirc(float x);

    float inBack(float x);
    float outBack(float x);
    float inOutBack(float x);

    float inElastic(float x);
    float outElastic(float x);
    float inOutElastic(float x);

    float inBounce(float x);
    float outBounce(float x);
    float inOutBounce(float x);
}