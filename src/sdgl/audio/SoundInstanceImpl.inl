#pragma once
#include "SoundInstance.h"
#include "fmod.h"

#include <sdgl/assert.h>

namespace sdgl {
    struct SoundInstance::Impl {
        Impl(FMOD::Sound *sound, FMOD::ChannelGroup *group, bool paused) : sound(sound), group(group), channel()
        {
            // Only instantiate channel when user demands it to be played
            instantiate(paused);
        }

        FMOD::Sound *sound;
        FMOD::ChannelGroup *group;
        FMOD::Channel *channel;

        /// FMOD channel callback for SoundInstance::Impl
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

            // Get impl object
            SoundInstance::Impl *impl;
            auto result = channel->getUserData((void **)&impl);
            if (result != FMOD_OK)
            {
                return result;
            }

            // Apply callback
            switch(callbackType)
            {
                case FMOD_CHANNELCONTROL_CALLBACK_END:
                {
                    impl->channel = nullptr;
                } break;
                default:
                {

                } break;
            }
            return FMOD_OK;
        }

        void instantiate(bool startPaused)
        {
            FMOD::System *sys;
            FMOD_CHECK(sound->getSystemObject(&sys));

            FMOD::Channel *channel;
            FMOD_CHECK(sys->playSound(sound, group, true, &channel));
            FMOD_CHECK(channel->setCallback(fmodChannelCallback));
            FMOD_CHECK(channel->setUserData(this));
            if (!startPaused)
            {
                FMOD_CHECK(channel->setPaused(false));
            }

            this->channel = channel;
        }

        void play()
        {
            if (channel->setPosition(0, FMOD_TIMEUNIT_PCM) == FMOD_ERR_INVALID_HANDLE)
            {
                instantiate(true);
                FMOD_CHECK(channel->setPosition(0, FMOD_TIMEUNIT_PCM));
            }

            FMOD_CHECK(channel->setPaused(false));
        }

        void pause(bool value)
        {
            if (!channel || channel->setPaused(value) == FMOD_ERR_INVALID_HANDLE)
            {
                instantiate(value);
            }
        }

        [[nodiscard]]
        bool isPaused() const
        {
            bool value;
            if (!channel || channel->getPaused(&value) == FMOD_ERR_INVALID_HANDLE) // sound ended or uninstantiated
                return true;
            return value;
        }

        void position(float seconds)
        {
            float frequency;
            if (!channel || channel->getFrequency(&frequency) == FMOD_ERR_INVALID_HANDLE)
            {
                instantiate(true);
                FMOD_CHECK(channel->getFrequency(&frequency));
            }

            FMOD_CHECK(channel->setPosition(seconds * frequency, FMOD_TIMEUNIT_PCM));
        }

        [[nodiscard]]
        float length() const
        {
            float frequency;
            FMOD_CHECK(sound->getDefaults(&frequency, nullptr));
            SDGL_ASSERT(frequency > 0);

            unsigned int length;
            FMOD_CHECK(sound->getLength(&length, FMOD_TIMEUNIT_PCM));

            return (float)length / frequency;
        }

        /// @return current position, or a value less than 0 if the track is stopped (ended, stopped, not started, etc.)
        float position() const
        {
            float frequency;
            if (!channel || channel->getFrequency(&frequency) == FMOD_ERR_INVALID_HANDLE)
            {
                return -1.f;
            }

            SDGL_ASSERT(frequency > 0);

            unsigned int position;
            FMOD_CHECK(channel->getPosition(&position, FMOD_TIMEUNIT_PCM));
            return (float)position / frequency;
        }

    private:
        FMOD::System *getSystem()
        {
            FMOD::System *sys;
            FMOD_CHECK(sound->getSystemObject(&sys));
            return sys;
        }
    };
}
