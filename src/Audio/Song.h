#pragma once

#include <cinttypes>
#include <string>
#include <cstdio>

#include <al.h>
#include <alc.h>

#include <filesystem>

#include "System/Logger.h"

namespace Sourcehold {
    namespace Audio {
        class Song {
            const static uint32_t SAMPLING_RATE = 44100;
            ALuint source;
            ALuint buffer;
            uint8_t *ptr;
            size_t size;
            bool repeat, valid = false, fading = false;
            float gain;
            double fadeBase = 0.0, fadeAmount = 0.0;
            bool fadeIn;
        public:
            Song(const Song &source);
            Song(std::filesystem::path path, bool repeat = false);
            Song(uint8_t *ptr = NULL, size_t size = 0, bool repeat = false);
            ~Song();

            bool Load(std::filesystem::path path, bool repeat = false);
            bool Play();
            void Pause();
            void Resume();
            void Stop();
            void Rewind();
            void SetFadeOut(double amount = 1.0);
            void SetFadeIn(double amount = 1.0);
            void UpdateFade();
            void Destroy();

            bool IsPlaying();

            inline bool IsValid() {
                return valid;
            }
            inline bool IsFading() {
                return fading;
            }
            inline void SetGain(double g) {
                gain = g;
            }
            inline bool IsRepeating() {
                return repeat;
            }
        };
    }
}
