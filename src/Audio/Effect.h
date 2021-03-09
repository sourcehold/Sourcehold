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
            constexpr static uint8_t num_audio_buffers_ = 4;
            bool playing_;
            bool valid_ = false;
            bool looping_; 
            bool audioInit_;
            AVFormatContext *ic_;
            SwrContext *swr_;
            AVCodec *audio_decoder_;
            AVPacket audio_packet_;
            AVFrame *audio_frame_;
            AVCodecContext *audio_ctx_;
            ALuint al_source_;
            ALuint al_free_buffers_[num_audio_buffers_];
            ALuint al_buffers_[num_audio_buffers_];
            ALuint al_num_free_buffers_ = num_audio_buffers_;
            ALuint al_num_channels_;
            ALuint al_format_;
            ALuint al_sample_rate_;
            char *audio_buffer_;
            int audio_stream_;
            size_t size_;
        public:
            Effect();
            ~Effect();

            bool Load(ghc::filesystem::path path, bool looping = false);
            void Destroy();
            void Play();
            void Stop();
            void Update();

            inline bool IsPlaying() { return playing_; }
        protected:
        };
    }
}
