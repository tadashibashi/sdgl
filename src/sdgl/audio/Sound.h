#pragma once

namespace FMOD {
    class Sound;
    class ChannelGroup;
}

namespace sdgl {
    class Sound {

    };

    class SoundInstance {
    public:
        void pause(bool value);
        void play();

        [[nodiscard]]
        bool isPaused() const;

    private:
        friend class AudioEngine;
        friend void soundInstanceCallback(SoundInstance *inst, int callbackType, void *data1, void *data2);

        SoundInstance(FMOD::Sound *sound, FMOD::ChannelGroup *group, bool paused);
        ~SoundInstance();


        void processEnd();

        void provide(FMOD::Sound *sound, FMOD::ChannelGroup *channel, bool paused);
        FMOD::Sound *getSound();

        struct Impl;
        Impl *m;
    };
}