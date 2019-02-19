#pragma once

#include <cinttypes>
#include <string>
#include <cstdio>

#include <AL/al.h>
#include <AL/alc.h>

#include <Config.h>
#include <System/Logger.h>

namespace Sourcehold
{
    namespace Audio
    {
        class AudioSource
        {
                ALuint source; /* OpenAL audio source */
                ALuint buffer; /* OpenAL audio buffer */
                uint8_t *ptr; /* Audio buffer */
                size_t size; /* Buffer size */
                bool repeat, valid; /* Properties */
                float gain; /* Current gain */
                const uint32_t SAMPLING_RATE = 44100;
            public:
                AudioSource(const AudioSource &source);
                AudioSource(const std::string path, bool repeat = false);
                AudioSource(uint8_t *ptr = NULL, size_t size = 0, bool repeat = false);
                ~AudioSource();

                bool LoadSong(const std::string path, bool repeat = false);
                bool LoadEffect(const std::string path, bool repeat = false);
                bool Create(void *buffer, size_t size, bool repeat = false);
                bool Play();
                void Pause();
                void Resume();
                void Stop();

                bool IsValid();
                bool IsPlaying();
                bool IsRepeating();
            protected:
        };
    }
}
