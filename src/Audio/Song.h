#pragma once

#include <cinttypes>
#include <string>
#include <cstdio>

#include <al.h>
#include <alc.h>

#include "System/filesystem.h"

#include "System/Logger.h"

namespace Sourcehold {
    namespace Audio {
        class Song {
            constexpr static uint32_t sampling_rate_ = 44100;
            ALuint source_;
            ALuint buffer_;
            uint8_t *ptr_;
            size_t size_;
            bool repeat_;
            bool valid_ = false;
            bool fading_ = false;
            float gain_;
            double fade_base_ = 0.0;
            double fade_amount_ = 0.0;
            bool fade_in_;
        public:
            Song(const Song &source) = default;
            Song(ghc::filesystem::path path, bool repeat = false);
            Song(uint8_t *ptr = NULL, size_t size = 0, bool repeat = false);
            ~Song();

            bool Load(ghc::filesystem::path path, bool repeat = false);
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
                return valid_;
            }
            inline bool IsFading() {
                return fading_;
            }
            inline void SetGain(float g) {
                gain_ = g;
            }
            inline bool IsRepeating() {
                return repeat_;
            }
        };
    }
}
