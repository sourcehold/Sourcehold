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

#include <Sound/Sound.h>

namespace Sourcehold
{
    namespace Rendering
    {
        class BinkVideo
        {
                Context ctx;
                AVInputFormat *bink_input;
                AVCodec *bink_codec;
                AVFormatContext *ic;
                AVCodec *decoder, *audioDecoder;
                AVPacket packet;
                AVFrame *frame;
                AVCodecContext *codecCtx, *audioCtx;
                SwsContext *sws;
                SwrContext *swr;
                int videoStream, audioStream;
                double timebase = 0.0;
            public:
                BinkVideo();
                ~BinkVideo();

                bool Init(Context &ctx);
                bool LoadFromDisk(std::string path);
                void InitFramebuffer(Texture &texture);
                void Close();
                void Decode(Texture &video, Sound::AudioSource &audio);
            private:
        };
    }
}
