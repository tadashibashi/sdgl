#pragma once
#include <sdgl/sdglib.h>

namespace sdgl {
    class SoundInstance;

    class Sound {
    public:
        Sound();
        ~Sound();
        bool load(const fs::path &filepath);

        [[nodiscard]]
        bool isLoaded() const;

        [[nodiscard]]
        uint id() const;

        SoundInstance *createInstance();

    private:
        struct Impl;
        Impl *m;
    };
}