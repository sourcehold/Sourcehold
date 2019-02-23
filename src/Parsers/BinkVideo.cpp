#include <Parsers/BinkVideo.h>

using namespace Sourcehold::System;
using namespace Sourcehold::Audio;
using namespace Sourcehold::Rendering;

BinkVideo::BinkVideo() {

}

BinkVideo::~BinkVideo() {

}

bool BinkVideo::Init() {
    av_log_set_level(AV_LOG_ERROR);
    bink_input = av_find_input_format("bink");
    if(!bink_input) {
        Logger::error("RENDERING") << "Unable to find libavcodec input format 'bink'!" << std::endl;
        return false;
    }

    ic = avformat_alloc_context();
    sc = avformat_alloc_context();
    if(!ic || !sc) {
        Logger::error("RENDERING") << "Unable to allocate input format context!" << std::endl;
        return false;
    }

    return true;
}

bool BinkVideo::LoadFromDisk(std::string path) {
    int out = avformat_open_input(
        &ic,
        path.c_str(),
        bink_input,
        NULL
    );
    if(out < 0) {
        Logger::error("RENDERING") << "Unable to open bink video input stream: '" << path << "'!" << std::endl;
        return false;
    }

    out = avformat_open_input(
        &sc,
        path.c_str(),
        bink_input,
        NULL
    );
    if(out < 0) {
        Logger::error("RENDERING") << "Unable to open bink audio input stream: '" << path << "'!" << std::endl;
        return false;
    }

    sc->max_analyze_duration = ic->max_analyze_duration = 10000000;
    if(avformat_find_stream_info(ic, NULL) < 0) {
        Logger::error("RENDERING") << "Unable to get bink video stream info!" << std::endl;
        return false;
    };

    if(avformat_find_stream_info(sc, NULL) < 0) {
        Logger::error("RENDERING") << "Unable to get bink audio stream info!" << std::endl;
        return false;
    };

    videoStream = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);
    audioStream = av_find_best_stream(sc, AVMEDIA_TYPE_AUDIO, -1, -1, &audioDecoder, 0);
    if(videoStream < 0) {
        Logger::error("RENDERING") << "Unable to find bink video stream index!" << std::endl;
        return false;
    }

    if(audioStream >= 0) {
        audioDecoder = avcodec_find_decoder(sc->streams[audioStream]->codecpar->codec_id);
        if(!audioDecoder) {
            Logger::error("RENDERING") << "Unable to find bink video decoder!" << std::endl;
            return false;
        }

        audioCtx = avcodec_alloc_context3(audioDecoder);
        if(!audioCtx) {
            Logger::error("RENDERING") << "Unable to allocate audio codec context!" << std::endl;
            return false; 
        }

        avcodec_parameters_to_context(audioCtx, sc->streams[audioStream]->codecpar);
        int ca = avcodec_open2(audioCtx, audioDecoder, NULL);
        if(ca < 0) {
            Logger::error("RENDERING") << "Unable to initialize audio AVCodecContext!" << std::endl;
            return false;
        }

        swr = swr_alloc();
        av_opt_set_int(swr, "in_channel_count", audioCtx->channels, 0);
        av_opt_set_int(swr, "out_channel_count", 1, 0);
        av_opt_set_int(swr, "in_channel_layout", audioCtx->channel_layout, 0);
        av_opt_set_int(swr, "out_channel_layout", AV_CH_LAYOUT_MONO, 0);
        av_opt_set_int(swr, "in_sample_rate", audioCtx->sample_rate, 0);
        av_opt_set_int(swr, "out_sample_rate", 44100, 0);
        av_opt_set_sample_fmt(swr, "in_sample_fmt", audioCtx->sample_fmt, 0);
        av_opt_set_sample_fmt(swr, "out_sample_fmt", AV_SAMPLE_FMT_DBL,  0);
        swr_init(swr);

        if(!swr) {
            Logger::error("RENDERING") << "Unable to create swresample context!" << std::endl;
            return false;
        }

        hasAudio = true;
    }

    decoder = avcodec_find_decoder(ic->streams[videoStream]->codecpar->codec_id);
    if(!decoder) {
        Logger::error("RENDERING") << "Unable to find bink video decoder!" << std::endl;
        return false;
    }
    
    codecCtx = avcodec_alloc_context3(decoder);
    if(!codecCtx) {
        Logger::error("RENDERING") << "Unable to allocate codec context!" << std::endl;
        return false; 
    }

    avcodec_parameters_to_context(codecCtx, ic->streams[videoStream]->codecpar);

    timebase = av_q2d(ic->streams[videoStream]->time_base);

    uint8_t bink_extradata[4] = { 0 } ;
    codecCtx->extradata = bink_extradata;
    codecCtx->extradata_size = sizeof(bink_extradata);

    int cv = avcodec_open2(codecCtx, decoder, NULL);
    if(cv < 0) {
        Logger::error("RENDERING") << "Unable to initialize video AVCodecContext!" << std::endl;
        return false;
    }

    av_init_packet(&packet);

    frame = av_frame_alloc();
    if(!frame) {
        Logger::error("RENDERING") << "Unable to allocate libavcodec frame!" << std::endl;
        return false;
    }

    sws = sws_getContext(
        codecCtx->width,
        codecCtx->height,
        codecCtx->pix_fmt,
        800, 600,
        AV_PIX_FMT_RGB32,
        SWS_BILINEAR,
        NULL,
        NULL,
        NULL
    );
    if(!sws) {
        Logger::error("RENDERING") << "Unable to create swscale context!" << std::endl;
        return false;
    }

    return true;
}

void BinkVideo::InitFramebuffer(Texture &texture) {
    texture.AllocNew(800, 600, SDL_PIXELFORMAT_RGB888);
}

void BinkVideo::Decode(Texture &video, AudioSource &audio) {
    int ret;
    if(av_read_frame(ic, &packet) >= 0) {
        if(packet.stream_index == videoStream) {
            ret = avcodec_send_packet(codecCtx, &packet);
            if(ret) {
                Logger::error("RENDERING") << "An error occured during bink video decoding!" << std::endl;
                return;
            }

            while(ret > 0) {
                ret = avcodec_receive_frame(codecCtx, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    return;
                }

                long wait_until = packet.pts * timebase;
                //SDL_Delay(wait_until);

                uint32_t dst[800 * 600];
                uint8_t* slices[3] = {(uint8_t*)&dst[0], 0, 0};
                int strides[3] = {800*4, 0, 0};

                sws_scale(sws, frame->data, frame->linesize, 0, codecCtx->height, slices, strides);
                memcpy(video.GetData(), dst, 800*600*4);

                video.UpdateTexture();
            }
        }else if(packet.stream_index == audioStream) {
            
        }
        av_packet_unref(&packet);
    }
}

void BinkVideo::Close() {
    avformat_close_input(&ic);
    avformat_close_input(&sc);
    av_frame_free(&frame);
    decoder->close(codecCtx);
    av_free(codecCtx);
}
