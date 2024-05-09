#include "Sound.h"
#include "fmod.h"

namespace sdgl {
    // friend callback, used to access SoundInstance internals
    void soundInstanceCallback(SoundInstance *inst, int callbackType, void *data1, void *data2)
    {
        switch(callbackType)
        {
            case FMOD_CHANNELCONTROL_CALLBACK_END:
            {
                inst->processEnd();
            } break;
            default:
            {

            } break;
        }
    }

    static FMOD_RESULT F_CALL fmodChannelCallback(
        FMOD_CHANNELCONTROL *channelControl,
        FMOD_CHANNELCONTROL_TYPE type,
        FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType,
        void *data1,
        void *data2
        )
    {
        // Channel type will always be Channel
        auto channel = reinterpret_cast<FMOD::Channel *>(channelControl);
        SoundInstance *instance;
        auto result = channel->getUserData((void **)&instance);
        if (result != FMOD_OK)
        {
            return result;
        }

        soundInstanceCallback(instance, callbackType, data1, data2);
        return FMOD_OK;
    }



    struct SoundInstance::Impl {
        Impl() : sound(), channel(), group()
        { }

        FMOD::Sound *sound;
        FMOD::ChannelGroup *group;
        FMOD::Channel *channel;
    };

    void SoundInstance::play()
    {
        pause(false);
    }

    void SoundInstance::processEnd()
    {
        m->channel = nullptr;
    }

    void SoundInstance::pause(bool value)
    {
        auto result = m->channel->setPaused(value);
        if (result == FMOD_ERR_INVALID_HANDLE)
        {
            FMOD::System *sys;
            FMOD_CHECK(m->sound->getSystemObject(&sys));
            FMOD_CHECK(sys->playSound(m->sound, m->group, true, &m->channel));
            FMOD_CHECK(m->channel->setCallback(fmodChannelCallback));
            FMOD_CHECK(m->channel->setUserData(this));
            FMOD_CHECK(m->channel->setPaused(false));
        }
    }

    bool SoundInstance::isPaused() const
    {
        if (!m->channel) // sound ended or uninstantiated
            return true;

        bool value;
        const auto result = m->channel->isPlaying(&value);
        if (result == FMOD_ERR_INVALID_HANDLE)
        {
            return true;
        }

        return value;
    }

    SoundInstance::SoundInstance(FMOD::Sound *sound, FMOD::ChannelGroup *group, bool paused) : m(new Impl)
    {
        provide(sound, group, paused);
    }

    SoundInstance::~SoundInstance()
    {
        delete m;
    }

    // constructor-like, sets up the instance
    void SoundInstance::provide(FMOD::Sound *sound, FMOD::ChannelGroup *group, bool paused)
    {
        if (sound)
        {
            FMOD::System *sys;
            FMOD_CHECK(sound->getSystemObject(&sys));

            FMOD::Channel *channel;
            FMOD_CHECK(sys->playSound(sound, group, true, &channel));
            FMOD_CHECK(channel->setCallback(fmodChannelCallback));
            FMOD_CHECK(channel->setUserData(this));
            if (!paused)
            {
                FMOD_CHECK(channel->setPaused(false));
            }

            m->sound = sound;
            m->channel = channel;
            m->group = group;
        }
    }

    FMOD::Sound *SoundInstance::getSound()
    {
        return m->sound;
    }
}
