#pragma once
#include "Sound.h"

namespace sdgl {
    class AudioEngine {
    public:
        AudioEngine();
        ~AudioEngine();

        bool init();
        void shutdown();

        SoundInstance *createSound(const fs::path &filepath);
        void destroySound(SoundInstance *sound);

        void update();
    private:
        struct Impl;
        Impl *m;
    };
}