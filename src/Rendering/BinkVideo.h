#pragma once

extern "C" {
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <al.h>
#include <alc.h>

#include <memory>

#include "System/Logger.h"

#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"

#include "System/filesystem.h"

namespace Sourcehold {
    namespace Rendering {
        /*
         * A single bink video file, exposes a texture as a frame buffer
         */
        class BinkVideo : public Texture {
            AVFormatContext *ic_;
            AVCodec *decoder, *audioDecoder_;
            AVPacket packet_;
            AVFrame *frame_, *audioFrame_;
            AVCodecContext *codecCtx_, *audioCtx_;
            SwsContext *sws_;
            const static uint32_t NUM_AUDIO_BUFFERS_ = 4;
            ALuint alSource_;
            ALuint alFreeBuffers_[NUM_AUDIO_BUFFERS_];
            ALuint alBuffers_[NUM_AUDIO_BUFFERS_];
            ALuint alNumFreeBuffers_ = NUM_AUDIO_BUFFERS_;
            ALuint amNumChannels_;
            ALuint alFormat_;
            ALuint anSampleRate_;
            char *audioBuffer_;
            int videoStream_, audioStream_, size_;
            float fps_;
            std::vector<uint32_t> framebuf_;
            int delayTimer_;
            bool packetFinished_;
            bool hasAudio_ = false;
            bool audioInit_ = false;
            bool looping_;
            bool running_ = false; 
            bool valid_ = false;
        public:
            BinkVideo();
            BinkVideo(ghc::filesystem::path path, bool looping = false);
            ~BinkVideo();

            bool LoadFromDisk(ghc::filesystem::path path, bool looping = false);
            void Close();
            void Update();

            inline bool IsRunning() {
                return running_;
            }
        protected:
            void test();
        };
    }
}
