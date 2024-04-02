#pragma once
#include <sdgl/sdglib.h>
#include "TweenFunction.h"

namespace sdgl
{
    /**
     * A function that should contain no side effects - input a normalized value and get one back
     */
    using TweenFunc = float(*)(float);

    class Tween {
    public:
        explicit Tween(float duration, TweenFunc func = tweenf::easeInOutQuad);
        ~Tween() = default;

        /**
         * Set callback which fires the frame the tween ends. If it is in yoyo mode, it will fire once it has returned
         * to the starting position.
         */
        Tween &onEnd(func<void()> callback);

        /**
         * Set callback which receives normalized value on each step (call to update)
         * @return reference to this Tween for chaining
         */
        Tween &onStep(func<void(float)> callback);

        /**
         * Set the speed multiplier
         * @return reference to this Tween for chaining
         */
        Tween &setSpeed(const float value)
        {
             m_speed = value;
             return *this;
        }

        /**
         * @return speed multiplier
         */
        [[nodiscard]]
        float getSpeed() const { return m_speed; }

        /**
         * Set whether the tween should behave like a yoyo. This means that once it has reached the end, it will then
         * start cycling backward until it returns to the initial value.
         */
        Tween &setYoyo(bool value) { m_isYoyo = value; return *this; }

        [[nodiscard]]
        bool isYoyo() const { return m_isYoyo; }

        [[nodiscard]]
        bool isReversing() const { return m_isReversing; }

        void start(float offsetSeconds = 0);

        [[nodiscard]]
        bool isPaused() const { return m_isPaused; }
        Tween &setPaused(bool value);

        /** Abruptly stop the tween and set current time back to 0. Does not call onEnd. */
        void stop();

        /**
         * To be called every frame
         * @param deltaTime - the amount of time in seconds that have passed since the
         * last call to update
         */
        void update(float deltaTime);

        /**
         * Get current value scaled between a start and end point
         * @param start - start value
         * @param end   - end value
         * @return value scaled between start and end
         */
        [[nodiscard]]
        float calculateValue(float start, float end) const;

        [[nodiscard]]
        float getCurrentTime() const { return m_currentTime; }

        [[nodiscard]]
        float getCurrentValue() const { return m_currentValue; }

    private:
        void updateValue();
        bool m_isYoyo;
        bool m_isReversing;
        bool m_isPaused;
        float m_speed;
        float m_duration;
        float m_currentTime;
        float m_currentValue;
        func<void()> m_onEnd;
        func<void(float)> m_onStep;
        TweenFunc m_tweenFunc;
    };
}
