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

#include <memory>

#include <Config.h>

#include <System/Logger.h>

#include <Rendering/Renderer.h>
#include <Rendering/Texture.h>

#include <Audio/AudioSource.h>

namespace Sourcehold
{
    namespace Rendering
    {
        using Audio::AudioSource;

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
         * and an AudioSource as an audio buffer. Expects a valid
         * avcodec context
         */
        class BinkVideo : public Texture, public AudioSource
        {
                AVFormatContext *ic;
                AVCodec *decoder, *audioDecoder;
                AVPacket packet;
                AVFrame *frame;
                AVCodecContext *codecCtx, *audioCtx;
                SwsContext *sws;
                SwrContext *swr;
                int videoStream, audioStream;
                double timebase = 0.0;
                bool hasAudio = false, looping, running = false, valid = false;
            public:
                BinkVideo(std::shared_ptr<Renderer> man);
                BinkVideo(std::shared_ptr<Renderer> man, const std::string &path, bool looping = false);
                ~BinkVideo();

                bool LoadFromDisk(const std::string &path, bool looping = false);
                void Close();
                void Decode();

                inline bool IsRunning() { return running; }
            private:
        };
    }
}
