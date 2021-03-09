#include <thread>
#include <chrono>
#include <algorithm>

#include "Audio/Effect.h"
#include "System/Logger.h"

using namespace Sourcehold::Audio;
using namespace Sourcehold::Game;
using namespace Sourcehold::System;

Effect::Effect()
{
    ic_ = avformat_alloc_context();
    if(!ic_) {
        Logger::error(AUDIO) << "Unable to allocate input format context!" << std::endl;
    }
}

Effect::~Effect()
{
    Destroy();
}

bool Effect::Load(ghc::filesystem::path path, bool looping)
{
    looping_ = looping;

    int out = avformat_open_input(
        &ic_,
        path.string().c_str(),
        GetAVInputFormat(),
        NULL
        );
    if(out < 0) {
        return false;
    }

    ic_->max_analyze_duration = 10000;
    if(avformat_find_stream_info(ic_, NULL) < 0) {
        return false;
    };

    audio_stream_ = av_find_best_stream(ic_, AVMEDIA_TYPE_AUDIO, -1, -1, &audio_decoder_, 0);
    if(audio_stream_ < 0) {
        return false;
    }

    audio_ctx_ = avcodec_alloc_context3(audio_decoder_);
    if(!audio_ctx_) {
        return false;
    }

    avcodec_parameters_to_context(audio_ctx_, ic_->streams[audio_stream_]->codecpar);
    int ca = avcodec_open2(audio_ctx_, audio_decoder_, NULL);
    if(ca < 0) {
        return false;
    }

    audio_frame_ = av_frame_alloc();

    valid_ = true;
    audioInit_ = false;
    return true;
}

void Effect::Destroy()
{
    if(valid_) {
        av_frame_free(&audio_frame_);
        alSourceStop(al_source_);
        alDeleteSources(1, &al_source_);
        alDeleteBuffers(num_audio_buffers_, al_buffers_);

        valid_ = false;
    }
}

void Effect::Play()
{
    playing_ = true;
}

void Effect::Stop()
{
    playing_ = false;
}

void Effect::Update()
{
    if(!playing_ || !valid_) return;

    av_init_packet(&audio_packet_);

    int ret = 0;
    if (av_read_frame(ic_, &audio_packet_) < 0) {
        if (looping_) {
            av_seek_frame(ic_, -1, 0, 0);
            if (av_read_frame(ic_, &audio_packet_) < 0) {
                return;
            }
        }
        else {
            playing_ = false;
            return;
        }
    }

    av_frame_unref(audio_frame_);

    ret = avcodec_send_packet(audio_ctx_, &audio_packet_);

/*
  double dur_ms = (double)audioPacket.duration * av_q2d(audioCtx->time_base);
  std::this_thread::sleep_for(std::chrono::milliseconds((uint64_t)dur_ms));
*/

    while (ret >= 0) {
        ret = avcodec_receive_frame(audio_ctx_, audio_frame_);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        }

        if (!audioInit_) {
            alGenSources(1, &al_source_);
            Audio::PrintError();
            alGenBuffers(num_audio_buffers_, al_buffers_);
            Audio::PrintError();

            alSource3f(al_source_, AL_POSITION, 0.0f, 0.0f, 0.0f);
            alSource3f(al_source_, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
            alSourcef(al_source_, AL_PITCH, 1.0f);
            alSourcef(al_source_, AL_GAIN, 1.0f);

            al_format_ = AL_FORMAT_MONO16;
            al_num_channels_ = 1;

            al_num_free_buffers_ = num_audio_buffers_;

            std::copy(al_buffers_, al_buffers_ + num_audio_buffers_, al_free_buffers_);

            al_sample_rate_ = 22050;
            size_ = al_num_channels_ * static_cast<unsigned int>(audio_frame_->nb_samples) * static_cast<unsigned int>(av_get_bytes_per_sample(static_cast<AVSampleFormat>(audio_frame_->format)));
            audio_buffer_ = static_cast<char*>(std::malloc(size_));

            swr_ = swr_alloc_set_opts(
                NULL,
                AV_CH_LAYOUT_MONO,
                AV_SAMPLE_FMT_S16,
                static_cast<int>(al_sample_rate_),
                /*audioFrame->channel_layout*/AV_CH_LAYOUT_MONO,
                static_cast<AVSampleFormat>(audio_ctx_->sample_fmt),
                audio_ctx_->sample_rate,
                0,
                NULL
                );
            if(!swr_) {
                Logger::error(AUDIO) << "Unable to allocate swresample context!" << std::endl;
                return;
            }

            if(swr_init(swr_) < 0) {
                Logger::error(AUDIO) << "Unable to init swresample context!" << std::endl;
                return;
            }

            audioInit_ = true;
        }
        std::memset(audio_buffer_, 0, size_);

        int buffersFinished = 0;
        alGetSourcei(al_source_, AL_BUFFERS_PROCESSED, &buffersFinished);
        Audio::PrintError();

        if (buffersFinished > 0) {
            alSourceStop(al_source_);

            for (; buffersFinished > 0; buffersFinished--) {
                ALuint buffer = 0;
                alSourceUnqueueBuffers(al_source_, 1, &buffer);
                Audio::PrintError();

                if (buffer > 0) {
                    al_free_buffers_[al_num_free_buffers_] = buffer;
                    Audio::PrintError();
                    al_num_free_buffers_++;
                }
            }

            alSourcePlay(al_source_);
        }

        if (al_num_free_buffers_ > 0) {
            ALuint alBuffer = al_free_buffers_[al_num_free_buffers_ - 1];

            int smp = swr_convert(
                swr_,
                reinterpret_cast<uint8_t**>(audio_buffer_),
                static_cast<int>(size_),
                const_cast<const uint8_t**>(audio_frame_->extended_data),
                audio_frame_->nb_samples
                );

            if(smp < 0) {
                Logger::error(AUDIO) << "Sample conversion failed!" << std::endl;
                return;
            }

            alSourceStop(al_source_);

            alBufferData(alBuffer, static_cast<ALenum>(al_format_), audio_buffer_, smp*2, static_cast<ALsizei>(al_sample_rate_));
            Audio::PrintError();
            alSourceQueueBuffers(al_source_, 1, &alBuffer);
            Audio::PrintError();

            alSourcePlay(al_source_);

            al_num_free_buffers_--;
            al_free_buffers_[al_num_free_buffers_] = 0;
        }
    }
}
