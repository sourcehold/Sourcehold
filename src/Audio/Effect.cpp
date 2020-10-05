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
    ic = avformat_alloc_context();
    if(!ic) {
        Logger::error(AUDIO) << "Unable to allocate input format context!" << std::endl;
    }
}

Effect::~Effect()
{
    Destroy();
}

bool Effect::Load(ghc::filesystem::path path, bool looping)
{
    this->looping = looping;

    int out = avformat_open_input(
        &ic,
        path.string().c_str(),
        GetAVInputFormat(),
        NULL
        );
    if(out < 0) {
        return false;
    }

    ic->max_analyze_duration = 10000;
    if(avformat_find_stream_info(ic, NULL) < 0) {
        return false;
    };

    audioStream = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, &audioDecoder, 0);
    if(audioStream < 0) {
        return false;
    }

    audioCtx = avcodec_alloc_context3(audioDecoder);
    if(!audioCtx) {
        return false;
    }

    avcodec_parameters_to_context(audioCtx, ic->streams[audioStream]->codecpar);
    int ca = avcodec_open2(audioCtx, audioDecoder, NULL);
    if(ca < 0) {
        return false;
    }

    audioFrame = av_frame_alloc();

    valid = true;
    audioInit = false;
    return true;
}

void Effect::Destroy()
{
    if(valid) {
        av_frame_free(&audioFrame);
        alSourceStop(alSource);
        alDeleteSources(1, &alSource);
        alDeleteBuffers(NUM_AUDIO_BUFFERS, alBuffers);

        valid = false;
    }
}

void Effect::Play()
{
    playing = true;
}

void Effect::Stop()
{
    playing = false;
}

void Effect::Update()
{
    if(!playing || !valid) return;

    av_init_packet(&audioPacket);

    int ret = 0;
    if (av_read_frame(ic, &audioPacket) < 0) {
        if (looping) {
            av_seek_frame(ic, -1, 0, 0);
            if (av_read_frame(ic, &audioPacket) < 0) {
                return;
            }
        }
        else {
            playing = false;
            return;
        }
    }

    av_frame_unref(audioFrame);

    ret = avcodec_send_packet(audioCtx, &audioPacket);

/*
  double dur_ms = (double)audioPacket.duration * av_q2d(audioCtx->time_base);
  std::this_thread::sleep_for(std::chrono::milliseconds((uint64_t)dur_ms));
*/

    while (ret >= 0) {
        ret = avcodec_receive_frame(audioCtx, audioFrame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        }

        if (!audioInit) {
            alGenSources(1, &alSource);
            Audio::PrintError();
            alGenBuffers(NUM_AUDIO_BUFFERS, alBuffers);
            Audio::PrintError();

            alSource3f(alSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
            alSource3f(alSource, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
            alSourcef(alSource, AL_PITCH, 1.0f);
            alSourcef(alSource, AL_GAIN, 1.0f);

            alFormat = AL_FORMAT_MONO16;
            alNumChannels = 1;

            alNumFreeBuffers = NUM_AUDIO_BUFFERS;

            std::copy(alBuffers, alBuffers + NUM_AUDIO_BUFFERS, alFreeBuffers);

            alSampleRate = 22050;
            size = alNumChannels * audioFrame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)audioFrame->format);
            audioBuffer = (char*)std::malloc(size);

            swr = swr_alloc_set_opts(
                NULL,
                AV_CH_LAYOUT_MONO,
                AV_SAMPLE_FMT_S16,
                alSampleRate,
                /*audioFrame->channel_layout*/AV_CH_LAYOUT_MONO,
                (AVSampleFormat)audioCtx->sample_fmt,
                audioCtx->sample_rate,
                0,
                NULL
                );
            if(!swr) {
                Logger::error(AUDIO) << "Unable to allocate swresample context!" << std::endl;
                return;
            }

            if(swr_init(swr) < 0) {
                Logger::error(AUDIO) << "Unable to init swresample context!" << std::endl;
                return;
            }

            audioInit = true;
        }
        std::memset(audioBuffer, 0, size);

        int buffersFinished = 0;
        alGetSourcei(alSource, AL_BUFFERS_PROCESSED, &buffersFinished);
        Audio::PrintError();

        if (buffersFinished > 0) {
            alSourceStop(alSource);

            for (; buffersFinished > 0; buffersFinished--) {
                ALuint buffer = 0;
                alSourceUnqueueBuffers(alSource, 1, &buffer);
                Audio::PrintError();

                if (buffer > 0) {
                    alFreeBuffers[alNumFreeBuffers] = buffer;
                    Audio::PrintError();
                    alNumFreeBuffers++;
                }
            }

            alSourcePlay(alSource);
        }

        if (alNumFreeBuffers > 0) {
            ALuint alBuffer = alFreeBuffers[alNumFreeBuffers - 1];

            int smp = swr_convert(
                swr,
                (uint8_t**)audioBuffer,
                size,
                (const uint8_t**)audioFrame->extended_data,
                audioFrame->nb_samples
                );

            if(smp < 0) {
                Logger::error(AUDIO) << "Sample conversion failed!" << std::endl;
                return;
            }

            alSourceStop(alSource);

            alBufferData(alBuffer, alFormat, audioBuffer, smp*2, alSampleRate);
            Audio::PrintError();
            alSourceQueueBuffers(alSource, 1, &alBuffer);
            Audio::PrintError();

            alSourcePlay(alSource);

            alNumFreeBuffers--;
            alFreeBuffers[alNumFreeBuffers] = 0;
        }
    }
}
