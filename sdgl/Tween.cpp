#include "Tween.h"

#include <sdgl/logging.h>
#include <sdgl/math/mathf.h>

namespace sdgl {
    Tween::Tween(const float duration, const TweenFunc func) :
        m_isYoyo(false), m_isReversing(false), m_isPaused(false),
        m_speed(1.f), m_duration(duration), m_onEnd(), m_onStep(), m_tweenFunc(func),
        m_currentTime(0), m_currentValue()
    {
        SDGL_ASSERT(duration >= 0);
        SDGL_ASSERT(func);
        updateValue();
    }

    Tween &Tween::onEnd(func<void()> callback)
    {
        m_onEnd = std::move(callback);
        return *this;
    }

    Tween &Tween::onStep(func<void(float)> callback)
    {
        m_onStep = std::move(callback);
        return *this;
    }

    void Tween::start(float offsetSeconds)
    {
        m_isReversing = false;
        m_isPaused = false;
        m_currentTime = mathf::clamp(offsetSeconds, 0, m_duration);
        updateValue();

        if (m_onStep)
            m_onStep(m_currentValue);
    }

    Tween & Tween::setPaused(const bool value)
    {
        m_isPaused = value;

        // on resume
        if (value != m_isPaused && value == true)
        {
            if (m_onStep)
                m_onStep(m_currentValue);
        }

        return *this;
    }

    void Tween::stop()
    {
        m_isReversing = false;
        m_currentTime = 0;
        m_currentValue = m_tweenFunc(0);
        m_isPaused = true;
    }

    void Tween::update(const float deltaTime)
    {
        if (m_isPaused) return;

        // update time
        m_currentTime += deltaTime * m_speed * (m_isReversing ? -1.f : 1.f);

        bool didEnd = false;

        // time bounds check
        if (m_currentTime >= m_duration)
        {
            if (m_isYoyo && !m_isReversing)
            {
                m_isReversing = true;
                m_currentTime = m_duration - (m_currentTime - m_duration);
            }
            else
            {
                m_isPaused = true;
                m_currentTime = m_duration;
                if (m_onEnd)
                    didEnd = true;
            }
        }

        if (m_currentTime <= 0)
        {
            m_currentTime = 0;
            if (m_isYoyo && m_isReversing)
            {
                m_isReversing = false;
                m_isPaused = true;
                if (m_onEnd)
                    didEnd = true;
            }
        }

        updateValue();

        // fire step callback
        if (m_onStep)
            m_onStep(m_currentValue);

        // fire end callback
        if (didEnd && m_onEnd)
            m_onEnd();
    }

    float Tween::calculateValue(float start, float end) const
    {
        return m_currentValue * (end - start) + start;
    }

    void Tween::updateValue()
    {
        m_currentValue = m_tweenFunc(m_currentTime / m_duration);
    }
}
