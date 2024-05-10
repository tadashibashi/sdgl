#pragma once
#include <sdgl/sdglib.h>

namespace FMOD {
    class Sound;
    class ChannelGroup;
}

namespace sdgl {

    class SoundInstance {
    public:
        /// Pause or unpause the sound. If this sound already ended, and `false` is passed,
        /// it will restart sound from the beginning.
        /// @param value `true` = pause, `false` = unpause
        void pause(bool value);

        /// Start playing sound from beginning
        void play();

        void position(double seconds);

        [[nodiscard]]
        double position() const;

        /// Whether sound is paused / not playing. If sound ended, `true` will be returned.
        [[nodiscard]]
        bool isPaused() const;

    private:
        friend class AudioEngine;
        SoundInstance(FMOD::Sound *sound, FMOD::ChannelGroup *group, bool paused);
        ~SoundInstance();

        struct Impl;
        Impl *m;
    };
}