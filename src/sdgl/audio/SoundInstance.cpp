#include "SoundInstance.h"
#include "Sound.h"
#include "al.h"

namespace sdgl {
    struct SoundInstance::Impl {
        Impl(Sound *sound, bool paused) : m_sound(sound), m_source(0)
        {
            ALuint source = 0;
            alGenSources(1, &source);
            if (alCheck())
            {
                alSourcef(source, AL_PITCH, 1.f); alCheck();
                alSourcef(source, AL_GAIN, 1.f); alCheck();
                alSource3f(source, AL_POSITION, 0, 0, 0); alCheck();
                alSource3f(source, AL_VELOCITY, 0, 0, 0); alCheck();
                alSourcei(source, AL_LOOPING, AL_FALSE); alCheck();
                alSourcei(source, AL_BUFFER, sound->id()); alCheck();
                m_source = source;
            }
        }

        ~Impl()
        {
            if (m_source)
                alDeleteSources(1, &m_source);
        }

        Sound *m_sound;
        ALuint m_source;

        void play()
        {
            alSourcePlay(m_source); alCheck();
        }

        void position(float seconds)
        {
            int freq;
            alGetBufferi(m_sound->id(), AL_FREQUENCY, &freq); alCheck();
            alSourcef(m_source, AL_SAMPLE_OFFSET,
                static_cast<ALfloat>(seconds) / static_cast<ALfloat>(freq)); alCheck();
        }

        [[nodiscard]]
        float position() const
        {
            int offset;
            alGetSourcei(m_source, AL_SAMPLE_OFFSET, &offset); alCheck();
            int freq;
            alGetBufferi(m_sound->id(), AL_FREQUENCY, &freq); alCheck();

            return static_cast<float>(offset) / static_cast<float>(freq);
        }

        void pause(bool value)
        {
            if (value)
            {
                alSourcePause(m_source); alCheck();
            }
            else
            {
                alSourcePlay(m_source); alCheck();
            }

        }

        [[nodiscard]]
        bool isPaused() const
        {
            ALint paused;
            alGetSourcei(m_source, AL_PAUSED, &paused); alCheck();
            return paused;
        }

        [[nodiscard]]
        float pitch() const
        {
            float value;
            alGetSourcef(m_source, AL_PITCH, &value); alCheck();
            return value;
        }

        void pitch(float value)
        {
            alSourcef(m_source, AL_PITCH, value); alCheck();
        }

        [[nodiscard]]
        bool looping() const
        {
            ALint value;
            alGetSourcei(m_source, AL_LOOPING, &value); alCheck();
            return value;
        }

        void looping(bool value)
        {
            alSourcei(m_source, AL_LOOPING, value); alCheck();
        }

        [[nodiscard]]
        float gain() const
        {
            float value;
            alGetSourcef(m_source, AL_GAIN, &value); alCheck();
            return value;
        }

        void gain(float value)
        {
            alSourcef(m_source, AL_GAIN, value); alCheck();
        }
    };

    void SoundInstance::play()
    {
        m->play();
    }

    void SoundInstance::position(float seconds)
    {
        m->position(seconds);
    }

    float SoundInstance::position() const
    {
        return m->position();
    }

    void SoundInstance::pause(bool value)
    {
        m->pause(value);
    }

    bool SoundInstance::isPaused() const
    {
        return m->isPaused();
    }

    float SoundInstance::pitch() const
    {
        return m->pitch();
    }

    void SoundInstance::pitch(float value)
    {
        m->pitch(value);
    }

    float SoundInstance::gain() const
    {
        return m->gain();
    }

    void SoundInstance::gain(float value)
    {
        m->gain(value);
    }

    bool SoundInstance::looping() const
    {
        return m->looping();
    }

    void SoundInstance::looping(bool value)
    {
        m->looping(value);
    }

    SoundInstance::SoundInstance(Sound *sound, bool paused) : m(new Impl(sound, paused))
    {}

    SoundInstance::~SoundInstance()
    {
        delete m;
    }
}
