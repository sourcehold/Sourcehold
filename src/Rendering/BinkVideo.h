#pragma once

extern "C" {
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc  avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif
}

#include <al.h>
#include <alc.h>

#include <memory>

#include <Config.h>

#include <System/Logger.h>

#include <Rendering/Renderer.h>
#include <Rendering/Texture.h>

#include <boost/filesystem.hpp>

namespace Sourcehold
{
    namespace Rendering
    {
        /*
         * Init the avcodec context needed by the BinkVideo class
         */
        bool InitAvcodec();

        /*
         * Destroy a previously created avcodec context and free
         * associated resources
         */
        void DestroyAvcodec();

        /*
         * A single bink video file, exposes a texture as a frame buffer
         */
        class BinkVideo : public Texture
        {
            AVFormatContext *ic;
            AVCodec *decoder, *audioDecoder;
            AVPacket packet;
            AVFrame *frame, *audioFrame;
            AVCodecContext *codecCtx, *audioCtx;
            SwsContext *sws;
            SwrContext *swr;
            const static uint32_t NUM_AUDIO_BUFFERS = 4;
            ALuint alSource;
            ALuint alFreeBuffers[NUM_AUDIO_BUFFERS];
            ALuint alBuffers[NUM_AUDIO_BUFFERS];
            ALuint alNumFreeBuffers = NUM_AUDIO_BUFFERS;
            ALuint alNumChannels;
            ALuint alFormat;
            ALuint alSampleRate;
            char *audioBuffer;
            int videoStream, audioStream;
            uint32_t lastTicks;
            float fps;
			uint32_t *framebuf;
            bool hasAudio = false, audioInit = false, looping, running = false, valid = false;
        public:
            BinkVideo(std::shared_ptr<Renderer> man);
            BinkVideo(std::shared_ptr<Renderer> man, boost::filesystem::path path, bool looping = false);
            ~BinkVideo();

            bool LoadFromDisk(boost::filesystem::path path, bool looping = false);
            void Close();
            void Update();

            inline bool IsRunning() { return running; }
        protected:
			void test();
        };
    }
}
