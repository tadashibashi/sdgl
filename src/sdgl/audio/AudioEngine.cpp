#include "AudioEngine.h"
#include "Sound.h"
#include "al.h"

#include <sdgl/logging.h>
#include <sdgl/sdglib.h>


namespace sdgl {
    struct AudioEngine::Impl {
        Impl() : m_sounds(), m_device(), m_context() { }
        ~Impl()
        {
            close();
        }

        map<fs::path::string_type, Sound *> m_sounds;
        ALCdevice *m_device;
        ALCcontext *m_context;

        bool init()
        {
            const auto audioDevice = alcOpenDevice(nullptr);
            if (!alcCheck(nullptr) || !audioDevice)
            {
                return false;
            }

            const auto audioContext = alcCreateContext(audioDevice, nullptr);
            if (!alcCheck(audioDevice) || !audioContext)
            {
                alcCloseDevice(audioDevice);
                return false;
            }
            alcMakeContextCurrent(audioContext); alcCheck(audioDevice);

            close();
            m_device = audioDevice;
            m_context = audioContext;
            return true;
        }

        void close()
        {
            if (!m_sounds.empty())
            {
                for (auto &[path, sound] : m_sounds)
                {
                    delete sound;
                }
                m_sounds.clear();
            }

            if (m_context)
            {
                alcMakeContextCurrent(nullptr);
                alcDestroyContext(m_context);
                m_context = nullptr;
            }

            if (m_device)
            {
                alcCloseDevice(m_device);
                m_device = nullptr;
            }
        }
    };

    AudioEngine::AudioEngine() : m(new Impl)
    {

    }

    AudioEngine::~AudioEngine()
    {
        delete m;
    }

    bool AudioEngine::init()
    {
        return m->init();
    }

    void AudioEngine::update()
    {

    }

    void AudioEngine::shutdown()
    {
        m->close();
    }

    SoundInstance *AudioEngine::createSound(const fs::path &filepath)
    {
        auto it = m->m_sounds.find(filepath.native());
        if (it != m->m_sounds.end())
        {
            return new SoundInstance(it->second, true); // TODO: store sound instances in Sound?
        }

        auto sound = new Sound();
        if (!sound->load(filepath))
        {
            delete sound;
            return nullptr;
        }

        m->m_sounds[filepath.native()] = sound;
        return new SoundInstance(sound, true); // TODO: store sound instances in Sound?
    }

    void AudioEngine::destroySound(SoundInstance *sound)
    {
        delete sound; // TODO: remove sound instance from Sound?
    }
}
