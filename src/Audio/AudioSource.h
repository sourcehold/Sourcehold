#pragma once

#include <cinttypes>
#include <string>
#include <cstdio>

#include <SDL.h>

#include <al.h>
#include <alc.h>

#include <System/Logger.h>

#include <boost/filesystem.hpp>

namespace Sourcehold
{
    namespace Audio
    {
        /*
         * A single audio source, assumes that the audio context
         * has been created already
         */
        class AudioSource
        {
            boost::filesystem::path path;
            ALuint source; /* OpenAL audio source */
            ALuint buffer; /* OpenAL audio buffer */
            uint8_t *ptr; /* Audio buffer */
            size_t size; /* Buffer size */
            bool repeat, valid = false, fading = false; /* Properties */
            float gain; /* Current gain */
            double fadeBase = 0.0, fadeAmount = 0.0;
            const uint32_t SAMPLING_RATE = 44100;
        public:
            AudioSource(const AudioSource &source);
            AudioSource(boost::filesystem::path path, bool repeat = false);
            AudioSource(uint8_t *ptr = NULL, size_t size = 0, bool repeat = false);
            ~AudioSource();

            bool LoadSong(boost::filesystem::path path, bool repeat = false);
            bool LoadEffect(boost::filesystem::path path, bool repeat = false);
            bool Create(void *buffer, size_t size, bool repeat = false);
            bool Play();
            void Pause();
            void Resume();
            void Stop();
            void SetFadeOut(double amount = 1.0);
            void UpdateFade();

            bool IsValid();
            bool IsPlaying();
            bool IsRepeating();
        protected:
        };
    }
}
