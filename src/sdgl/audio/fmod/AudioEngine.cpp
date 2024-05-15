#include "AudioEngine.h"
#include <sdgl/io/io.h>
#include <sdgl/logging.h>
#include <sdgl/sdglib.h>

#include "fmod.h"

namespace sdgl {

    struct AudioEngine::Impl {
        Impl() : m_system(), m_sounds(), m_instances()
        { }

        bool init()
        {
            FMOD::System *sys;
            auto result = FMOD::System_Create(&sys);
            if (result != FMOD_OK)
            {
                SDGL_ERROR("FMOD failed to create FMOD::System object: {}", FMOD_ErrorString(result));
                return false;
            }

            int samplerate;
            result = sys->getDriverInfo(0, nullptr, 0, nullptr, &samplerate, nullptr, nullptr);
            if (result != FMOD_OK)
            {
                SDGL_ERROR("FMOD failed to get driver info: {}", FMOD_ErrorString(result));
                return false;
            }

            result = sys->setSoftwareFormat(samplerate, FMOD_SPEAKERMODE_DEFAULT, 0);
            if (result != FMOD_OK)
            {
                SDGL_ERROR("FMOD failed to set software format: {}", FMOD_ErrorString(result));
                return false;
            }

#ifdef SDGL_PLATFORM_EMSCRIPTEN
            result = sys->setDSPBufferSize(2048, 2); // web browsers tend to perform better with a larger buffer
            if (result != FMOD_OK)
            {
                SDGL_ERROR("FMOD failed to set DSP buffer size: {}", FMOD_ErrorString(result));
                return false;
            }
#endif
            result = sys->init(1024, FMOD_INIT_NORMAL, nullptr);
            if (result != FMOD_OK)
            {
                SDGL_ERROR("FMOD::System failed to initialize: {}", FMOD_ErrorString(result));
                return false;
            }

            m_system = sys;
            return true;
        }

        FMOD::Sound *loadSound(const fs::path &filepath)
        {
            auto it = m_sounds.find(filepath.native());
            if (it != m_sounds.end())
            {
                return it->second;
            }

            FMOD::Sound *sound;
            const auto result = m_system->createSound(filepath.c_str(), 0, nullptr, &sound);
            if (result != FMOD_OK)
            {
                return nullptr;
            }

            m_sounds[filepath.native()] = sound;
            return sound;
        }

        void update()
        {
            FMOD_CHECK(m_system->update());
        }

        void shutdown()
        {
            for (auto &instance : m_instances)
            {
                delete instance;
            }
            m_instances.clear();

            for (auto &[path, sound] : m_sounds)
            {
                sound->release();
            }
            m_sounds.clear();

            if (m_system)
            {
                m_system->release();
                m_system = nullptr;
            }
        }

        FMOD::System *m_system;
        map<fs::path::string_type, FMOD::Sound *> m_sounds;
        set<SoundInstance *> m_instances;
    };

    AudioEngine::AudioEngine() : m(new Impl)
    {

    }

    AudioEngine::~AudioEngine()
    {
        shutdown();
        delete m;
    }

    bool AudioEngine::init()
    {
        return m->init();
    }

    void AudioEngine::shutdown()
    {
        m->shutdown();
    }

    SoundInstance *AudioEngine::createSound(const fs::path &filepath)
    {
        auto fullpath = filepath.is_absolute() ? filepath : io::getResourcePath() / filepath;
        auto sound = m->loadSound(fullpath);
        if (!sound) return nullptr;

        auto inst = new SoundInstance(sound, nullptr, true);

        m->m_instances.emplace(inst);
        return inst;
    }

    void AudioEngine::destroySound(SoundInstance *sound)
    {
        m->m_instances.erase(sound);
        delete sound;
    }

    void AudioEngine::update()
    {
        m->update();
    }
}
