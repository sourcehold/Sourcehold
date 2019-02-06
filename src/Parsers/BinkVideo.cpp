#include <Parsers/BinkVideo.h>

using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

BinkVideo::BinkVideo() {

}

BinkVideo::~BinkVideo() {

}

bool BinkVideo::Init(Context &ctx) {
    av_log_set_level(AV_LOG_DEBUG);
    bink_input = av_find_input_format("bink");
    if(!bink_input) {
        Logger::error("RENDERING") << "Unable to find libavcodec input format 'bink'!" << std::endl;
        return false;
    }

    ic = avformat_alloc_context();
    if(!ic) {
        Logger::error("RENDERING") << "Unable to allocate input format context!" << std::endl;
        return false;
    }

    this->ctx = ctx;
    return true;
}

bool BinkVideo::LoadFromDisk(boost::filesystem::path path) {
    int out = avformat_open_input(
        &ic,
        path.native().c_str(),
        bink_input,
        NULL
    );
    if(out < 0) {
        Logger::error("RENDERING") << "Unable to open bink input stream: '" << path.native() << "'!" << std::endl;
        return false;
    }

    avformat_find_stream_info(ic, NULL);
    streamn = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);

    decoder = avcodec_find_decoder(ic->streams[streamn]->codecpar->codec_id);
    codecCtx = avcodec_alloc_context3(decoder);
    avcodec_parameters_to_context(codecCtx, ic->streams[streamn]->codecpar);

    timebase = av_q2d(ic->streams[streamn]->time_base);

    uint8_t bink_extradata[4] = { 0 } ;
    codecCtx->extradata = bink_extradata;
    codecCtx->extradata_size = sizeof(bink_extradata);

    int a = avcodec_open2(codecCtx, decoder, NULL);
    if(a < 0) {
        Logger::error("RENDERING") << "Unable to initialize AVCodecContext!" << std::endl;
        return false;
    }

    av_init_packet(&packet);
    frame = av_frame_alloc();
    if(!frame) {
        Logger::error("RENDERING") << "Unable to allocate libavcodec frame!" << std::endl;
        return false;
    }

    sws = sws_getCachedContext(
        0,
        codecCtx->width,
        codecCtx->height,
        codecCtx->pix_fmt,
        800, 600,
        AV_PIX_FMT_RGB32,
        SWS_LANCZOS,
        0,
        0,
        0
    );
    if(!sws) {
        Logger::error("RENDERING") << "Unable to create swscale context!" << std::endl;
        return false;
    }

    return true;
}

Texture &BinkVideo::RenderFrame() {
    uint32_t begin_time = SDL_GetTicks();
    if(packet.stream_index == streamn) {
        int ret = avcodec_send_packet(codecCtx, &packet);
        if(ret < 0) {
            Logger::error("RENDERING") << "An error occured during bink decoding!" << std::endl;
            return texture;
        }

        avcodec_receive_frame(codecCtx, frame);
        long wait_until = begin_time + packet.pts * timebase;
        
        uint32_t dst[800 * 600] = { 0 };
        uint8_t* slices[3] = {(uint8_t*)&dst[0], 0, 0};
        int strides[3] = {800*4, 0, 0};

        sws_scale(sws, frame->data, frame->linesize, 0, codecCtx->height, slices, strides);
    }

    return texture;
}

void BinkVideo::Close() {
    avformat_close_input(&ic);
    av_frame_free(&frame);
    decoder->close(codecCtx);
    av_free(codecCtx);
}
