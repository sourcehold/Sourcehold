#pragma once

#include <boost/filesystem/fstream.hpp>

extern "C" {
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc  avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif
}

#include <Config.h>
#include <Rendering.h>
#include <Logger.h>

namespace OpenSH
{
    namespace Rendering
    {
        class BinkVideo
        {
                Context ctx;
                AVInputFormat *bink_input;
                AVCodec *bink_codec;
                AVFormatContext *ic;
                AVCodec *decoder;
                AVPacket packet;
                AVFrame *frame;
                AVCodecContext *codecCtx;
                SwsContext *sws;
                SDL_Texture *framebuffer;
                int streamn;
                double timebase = 0.0;
            public:
                BinkVideo();
                ~BinkVideo();

                bool Init(Context &ctx);
                bool LoadFromDisk(boost::filesystem::path path);
                void Close();
                SDL_Texture *RenderFrame();
            private:
        };
    }
}
