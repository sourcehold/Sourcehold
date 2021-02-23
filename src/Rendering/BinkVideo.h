#pragma once

extern "C" {
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#if defined(__APPLE__)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <al.h>
#include <alc.h>
#endif

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
            AVFormatContext *ic;
            AVCodec *decoder, *audioDecoder;
            AVPacket packet;
            AVFrame *frame, *audioFrame;
            AVCodecContext *codecCtx, *audioCtx;
            SwsContext *sws;
            const static uint32_t NUM_AUDIO_BUFFERS = 4;
            ALuint alSource;
            ALuint alFreeBuffers[NUM_AUDIO_BUFFERS];
            ALuint alBuffers[NUM_AUDIO_BUFFERS];
            ALuint alNumFreeBuffers = NUM_AUDIO_BUFFERS;
            ALuint alNumChannels;
            ALuint alFormat;
            ALuint alSampleRate;
            char *audioBuffer;
            int videoStream, audioStream, size;
            float fps;
            uint32_t *framebuf;
            int delayTimer;
            bool packetFinished;
            bool hasAudio = false, audioInit = false, looping, running = false, valid = false;
        public:
            BinkVideo();
            BinkVideo(ghc::filesystem::path path, bool looping = false);
            ~BinkVideo();

            bool LoadFromDisk(ghc::filesystem::path path, bool looping = false);
            void Close();
            void Update();

            inline bool IsRunning() {
                return running;
            }
        protected:
            void test();
        };
    }
}
