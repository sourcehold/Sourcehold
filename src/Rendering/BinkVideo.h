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

#include <Config.h>

#include <System/Logger.h>

#include <Rendering/Rendering.h>
#include <Rendering/Texture.h>

#include <Audio/AudioSource.h>

namespace Sourcehold
{
    namespace Rendering
    {
        class BinkVideo
        {
                AVInputFormat *bink_input;
                AVCodec *bink_codec;
                AVFormatContext *ic, *sc;
                AVCodec *decoder, *audioDecoder;
                AVPacket packet;
                AVFrame *frame;
                AVCodecContext *codecCtx, *audioCtx;
                SwsContext *sws;
                SwrContext *swr;
                int videoStream, audioStream;
                double timebase = 0.0;
                bool hasAudio = false;
            public:
                BinkVideo();
                ~BinkVideo();

                bool LoadFromDisk(std::string path);
                void InitFramebuffer(Texture &texture);
                void InitAudiobuffer(Audio::AudioSource &audiobuffer);
                void Close();
                void Decode(Texture &video, Audio::AudioSource &audio);
            private:
        };
    }
}
