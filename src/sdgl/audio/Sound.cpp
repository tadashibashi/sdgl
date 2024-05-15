#include "Sound.h"

#include <sdgl/io/io.h>
#include <sdgl/logging.h>

#include "al.h"
#include <SDL_audio.h>

namespace sdgl {

    static ALenum getFormat(const SDL_AudioSpec &spec)
    {
        static const auto HasFloat32 = alIsExtensionPresent("AL_EXT_FLOAT32");
        static const auto FormatMonoFloat32 = alGetEnumValue("AL_FORMAT_MONO_FLOAT32");
        static const auto FormatStereoFloat32 = alGetEnumValue("AL_FORMAT_STEREO_FLOAT32");

        if (spec.channels == 1 && spec.format == AUDIO_U8)
            return AL_FORMAT_MONO8;
        if (spec.channels == 1 && spec.format == AUDIO_S16SYS)
            return AL_FORMAT_MONO16;
        if (spec.channels == 2 && spec.format == AUDIO_U8)
            return AL_FORMAT_STEREO8;
        if (spec.channels == 2 && spec.format == AUDIO_S16SYS)
            return AL_FORMAT_STEREO16;
        if (spec.channels == 1 && spec.format == AUDIO_F32SYS)
            return HasFloat32 ? FormatMonoFloat32 : AL_NONE;
        if (spec.channels == 2 && spec.format == AUDIO_F32SYS)
            return HasFloat32 ? FormatStereoFloat32 : AL_NONE;
        return AL_NONE;
    }

    struct Sound::Impl {
        Impl() : m_buffer(0) {}
        ~Impl()
        {
            unload();
        }

        void unload()
        {
            if (m_buffer)
            {
                alDeleteBuffers(1, &m_buffer); alCheck();
                m_buffer = 0;
            }
        }

        [[nodiscard]]
        bool isLoaded() const { return m_buffer != 0; }

        bool load(const fs::path &filepath)
        {
            ALuint buffer;
            alGenBuffers(1, &buffer);
            if (!alCheck())
                return false;

            SDL_AudioSpec spec;
            ubyte *data;
            uint length;

            if (!SDL_LoadWAV((io::getResourcePath() / filepath).c_str(), &spec, &data, &length))
            {
                SDGL_ERROR("Failed to load WAV: {}", SDL_GetError());
                return false;
            }

            const auto audioFormat = getFormat(spec);
            alBufferData(buffer, audioFormat, data, length, spec.freq);
            if (!alCheck())
            {
                alDeleteBuffers(1, &buffer);
                SDL_free(data);
                return false;
            }

            SDL_free(data);

            if (m_buffer)
            {
                alDeleteBuffers(1, &m_buffer);
                alCheck(); // just display an error msg if problem
            }

            m_buffer = buffer;
            return true;
        }

        ALuint m_buffer;
    };

    Sound::Sound() : m(new Impl)
    {
    }

    Sound::~Sound()
    {
        delete m;
    }

    bool Sound::load(const fs::path &filepath)
    {
        return m->load(filepath);
    }

    bool Sound::isLoaded() const
    {
        return m->isLoaded();
    }

    uint Sound::id() const
    {
        return m->m_buffer;
    }

    SoundInstance *Sound::createInstance()
    {
        return nullptr;
    }

}

