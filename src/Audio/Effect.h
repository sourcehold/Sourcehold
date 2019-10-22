#pragma once

#include "System/filesystem.h"
#include <cinttypes>
#include <string>
#include <cstdio>

#include <al.h>
#include <alc.h>

#include "Audio/Audio.h"
#include "FFmpegUtil.h"

namespace Sourcehold {
    namespace Audio {
        class Effect {
            const static uint8_t NUM_AUDIO_BUFFERS = 4;
            bool playing, valid = false, looping, audioInit;
            AVFormatContext *ic;
            SwrContext *swr;
            AVCodec *audioDecoder;
            AVPacket audioPacket;
            AVFrame *audioFrame;
            AVCodecContext *audioCtx;
            ALuint alSource;
            ALuint alFreeBuffers[NUM_AUDIO_BUFFERS];
            ALuint alBuffers[NUM_AUDIO_BUFFERS];
            ALuint alNumFreeBuffers = NUM_AUDIO_BUFFERS;
            ALuint alNumChannels;
            ALuint alFormat;
            ALuint alSampleRate;
            char *audioBuffer;
            int audioStream, size;
        public:
            Effect();
            ~Effect();

            bool Load(ghc::filesystem::path path, bool looping = false);
            void Destroy();
            void Play();
            void Stop();
            void Update();

            inline bool IsPlaying() { return playing; }
        protected:
        };
    }
}
