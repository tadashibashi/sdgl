#include "SoundInstance.h"
#include "SoundInstanceImpl.inl"

namespace sdgl {

    void SoundInstance::play()
    {
        m->play();
    }

    void SoundInstance::position(double seconds)
    {
        m->position(seconds);
    }

    double SoundInstance::position() const
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

    SoundInstance::SoundInstance(FMOD::Sound *sound, FMOD::ChannelGroup *group, bool paused) :
        m(new Impl(sound, group, paused))
    {}

    SoundInstance::~SoundInstance()
    {
        delete m;
    }
}
