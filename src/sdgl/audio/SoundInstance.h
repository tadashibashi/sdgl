#pragma once
#include <sdgl/sdglib.h>

namespace sdgl {
    class Sound;

    class SoundInstance {
    public:
        /// Pause or unpause the sound. If this sound already ended, and `false` is passed,
        /// it will restart sound from the beginning.
        /// @param value `true` = pause, `false` = unpause
        void pause(bool value);

        /// Start playing sound from beginning
        void play();

        void position(float seconds);

        [[nodiscard]]
        float position() const;

        /// Whether sound is paused / not playing. If sound ended, `true` will be returned.
        [[nodiscard]]
        bool isPaused() const;

        [[nodiscard]]
        float pitch() const;
        void pitch(float value);
        [[nodiscard]]
        float gain() const;
        void gain(float value);
        [[nodiscard]]
        bool looping() const;
        void looping(bool value);

    private:
        friend class AudioEngine;
        SoundInstance(Sound *sound, bool paused);
        ~SoundInstance();

        struct Impl;
        Impl *m;
    };
}