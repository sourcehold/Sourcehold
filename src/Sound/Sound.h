#pragma once

#include <cinttypes>
#include <cstdio>
#include <cstring>
#include <vector>
#include <array>

#include <AL/al.h>
#include <AL/alc.h>

#include <Config.h>
#include <System/Logger.h>

namespace Sourcehold
{
    namespace Sound
    {
        /* Single audio source */
        struct AudioSource {
            ALuint source; /* OpenAL audio source */
            ALuint buffer; /* OpenAL audio buffer */
            uint8_t *ptr; /* Audio buffer */
            size_t size; /* Buffer size */
            bool repeat, playing, valid; /* Properties */
        };

        /* Sound super class */
        class SoundManager
        {
                /* OpenAL stuff */
                ALCdevice *device;
                ALCcontext *context;
            public:
                SoundManager();
                SoundManager(const SoundManager &snd);
                ~SoundManager();

                bool Init();

                AudioSource LoadSong(std::string path, bool repeat);
                AudioSource LoadEffect(std::string path, bool repeat);
                AudioSource CreateAudioSource(void *buffer, size_t size, bool repeat);

                void PlayAudio(AudioSource &source);
                bool IsPlaying();
            protected:
                void PrintError();
        };
    }
}
