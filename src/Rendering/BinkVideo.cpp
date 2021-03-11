#include <cstring>
#include <thread>
#include <chrono>

#include <SDL.h>

#include "Rendering/BinkVideo.h"
#include "Audio/Audio.h"
#include "FFmpegUtil.h"

using namespace Sourcehold;
using namespace System;
using namespace Audio;
using namespace Rendering;

BinkVideo::BinkVideo() : Texture()
{
    ic_ = avformat_alloc_context();
    if(!ic_) {
        Logger::error(RENDERING) << "Unable to allocate input format context!" << std::endl;
    }
}

BinkVideo::BinkVideo(ghc::filesystem::path path, bool looping) : Texture()
{
    ic_ = avformat_alloc_context();
    if(!ic_) {
        Logger::error(RENDERING) << "Unable to allocate input format context!" << std::endl;
    }

    LoadFromDisk(path, looping);
}

BinkVideo::~BinkVideo()
{
    Close();
}

bool BinkVideo::LoadFromDisk(ghc::filesystem::path path, bool is_looping)
{
    looping_ = is_looping;

    int out = avformat_open_input(
                  &ic_,
                  path.string().c_str(),
                  Game::GetAVInputFormat(),
                  NULL
              );
    if(out < 0) {
        return false;
    }

    ic_->max_analyze_duration = 10000;
    if(avformat_find_stream_info(ic_, NULL) < 0) {
        return false;
    };

    videoStream_ = av_find_best_stream(ic_, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);
    audioStream_ = av_find_best_stream(ic_, AVMEDIA_TYPE_AUDIO, -1, -1, &audioDecoder_, 0);
    if(videoStream_ < 0) {
        return false;
    }

    fps_ = static_cast<float>(ic_->streams[videoStream_]->avg_frame_rate.num) / static_cast<float>(ic_->streams[videoStream_]->avg_frame_rate.den);

    if(audioStream_ >= 0) {
        audioCtx_ = avcodec_alloc_context3(audioDecoder_);
        if(!audioCtx_) {
            return false;
        }

        avcodec_parameters_to_context(audioCtx_, ic_->streams[audioStream_]->codecpar);
        int ca = avcodec_open2(audioCtx_, audioDecoder_, NULL);
        if(ca < 0) {
            return false;
        }

        audioFrame_ = av_frame_alloc();

        hasAudio_ = true;
    }

    codecCtx_ = avcodec_alloc_context3(decoder);
    if(!codecCtx_) {
        return false;
    }

    avcodec_parameters_to_context(codecCtx_, ic_->streams[videoStream_]->codecpar);

    uint8_t bink_extradata[4] = { 0 } ;
    codecCtx_->extradata = bink_extradata;
    codecCtx_->extradata_size = sizeof(bink_extradata);

    int cv = avcodec_open2(codecCtx_, decoder, NULL);
    if(cv < 0) {
        return false;
    }

    frame_ = av_frame_alloc();
    if(!frame_) {
        return false;
    }

    sws_ = sws_getContext(
              codecCtx_->width,
              codecCtx_->height,
              codecCtx_->pix_fmt,
              codecCtx_->width,
              codecCtx_->height,
              AV_PIX_FMT_RGB32,
              SWS_BILINEAR,
              NULL,
              NULL,
              NULL
          );
    if(!sws_) {
        return false;
    }

    Texture::AllocNewStreaming(codecCtx_->width, codecCtx_->height, SDL_PIXELFORMAT_RGB888);
    framebuf_ = std::vector<uint32_t>(static_cast<size_t>(codecCtx_->width * codecCtx_->height));

    valid_ = true;
    running_ = true;
    packetFinished_ = true;
    delayTimer_ = 0;

    return true;
}

void BinkVideo::Close()
{
    if(valid_) {
        avformat_close_input(&ic_);
        av_frame_free(&frame_);
        decoder->close(codecCtx_);
        av_free(codecCtx_);

        if(hasAudio_) {
            decoder->close(audioCtx_);
            av_free(audioCtx_);
            av_frame_free(&audioFrame_);
            alSourceStop(alSource_);
            alDeleteSources(1, &alSource_);
            alDeleteBuffers(NUM_AUDIO_BUFFERS_, alBuffers_);
        }
    }
}

void BinkVideo::Update()
{
    if(!running_ || !valid_) return;

    av_init_packet(&packet_);

    int ret = 0;
    if (av_read_frame(ic_, &packet_) < 0) {
        if (looping_) {
            av_seek_frame(ic_, -1, 0, 0);
            if (av_read_frame(ic_, &packet_) < 0) {
                return;
            }
        }
        else {
            running_ = false;
            return;
        }
    }

    if (packet_.stream_index == videoStream_) {
        av_frame_unref(frame_);

        /* Receive new packet on start or if the last one was processed */
        if (packetFinished_) {
            if (avcodec_send_packet(codecCtx_, &packet_) < 0) return;
        }

        /* TODO: don't block the thread by sleeping and use internal
          timer or callback instead */
        uint64_t dur_ms = static_cast<uint64_t>(packet_.duration  * static_cast<int64_t>(av_q2d(codecCtx_->time_base)));
        std::this_thread::sleep_for(std::chrono::milliseconds(dur_ms));

        /* Receive one video frame */
        ret = avcodec_receive_frame(codecCtx_, frame_);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        }

        /* Decode video frame into framebuffer texture */
        uint8_t* slices[3] = { reinterpret_cast<uint8_t*>(&framebuf_[0]), 0, 0 };
        int strides[3] = { codecCtx_->width * 4, 0, 0 };

        Texture::LockTexture();

        sws_scale(sws_, frame_->data, frame_->linesize, 0, codecCtx_->height, slices, strides);
        std::memcpy(Texture::GetData(), framebuf_.data(), static_cast<uint64_t>(codecCtx_->width * codecCtx_->height) * sizeof(uint32_t));

        Texture::UnlockTexture();
    }
    else if (packet_.stream_index == audioStream_ && !IsOpenALMuted() && hasAudio_) {
        av_frame_unref(audioFrame_);

        /* Receive new packet on start or if the last one was processed */
        if (packetFinished_) {
            ret = avcodec_send_packet(audioCtx_, &packet_);
        }

        while (ret >= 0) {
            /* Receive one audio frame */
            ret = avcodec_receive_frame(audioCtx_, audioFrame_);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                return;
            }

            if (!audioInit_) {
                /* Init OpenAL stuff */
                alGenSources(1, &alSource_);
                Audio::PrintError();
                alGenBuffers(NUM_AUDIO_BUFFERS_, alBuffers_);
                Audio::PrintError();

                alSource3f(alSource_, AL_POSITION, 0.0f, 0.0f, 0.0f);
                alSource3f(alSource_, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
                alSourcef(alSource_, AL_PITCH, 1.0f);
                alSourcef(alSource_, AL_GAIN, 1.0f);

                /* Determine number of channels and format */
                if (audioFrame_->channel_layout == AV_CH_LAYOUT_MONO) {
                    alFormat_ = (audioFrame_->format == AV_SAMPLE_FMT_U8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
                    amNumChannels_ = 1;
                }
                else if (audioFrame_->channel_layout == AV_CH_LAYOUT_STEREO) {
                    alFormat_ = (audioFrame_->format == AV_SAMPLE_FMT_U8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
                    amNumChannels_ = 2;
                }
                else {
                    Logger::error(RENDERING) << "Bink audio channel layout is wrong!" << std::endl;
                    return;
                }

                /* Setup audio queue */
                alNumFreeBuffers_ = NUM_AUDIO_BUFFERS_;
                for (uint32_t i = 0; i < NUM_AUDIO_BUFFERS_; i++) {
                    alFreeBuffers_[i] = alBuffers_[i];
                }

                anSampleRate_ = static_cast<ALuint>(audioFrame_->sample_rate);
                size_ = static_cast<int32_t>(amNumChannels_) * audioFrame_->nb_samples * 2;
                audioBuffer_ = static_cast<char*>(std::malloc(static_cast<size_t>(size_)));

                audioInit_ = true;
            }
            std::memset(audioBuffer_, 0, static_cast<size_t>(size_));

            int buffersFinished = 0;
            alGetSourcei(alSource_, AL_BUFFERS_PROCESSED, &buffersFinished);
            Audio::PrintError();

            if (buffersFinished > 0) {
                alSourceStop(alSource_);

                for (; buffersFinished > 0; buffersFinished--) {
                    ALuint buffer = 0;
                    alSourceUnqueueBuffers(alSource_, 1, &buffer);
                    Audio::PrintError();

                    if (buffer > 0) {
                        alFreeBuffers_[alNumFreeBuffers_] = buffer;
                        Audio::PrintError();
                        alNumFreeBuffers_++;
                    }
                }

                alSourcePlay(alSource_);
            }

            if (alNumFreeBuffers_ > 0) {
                /**
                * TODO: Other versions of Stronghold might include
                * different audio formats (investigate!)
                */
                if (audioFrame_->format != AV_SAMPLE_FMT_FLT) {
                    return;
                }

                ALuint alBuffer = alFreeBuffers_[alNumFreeBuffers_ - 1];
                uint32_t numSamples = static_cast<uint32_t>(audioFrame_->nb_samples) * amNumChannels_;
                uint32_t dataSize = numSamples * 2;

                /* Convert samples */
                float *src = reinterpret_cast<float*>(audioFrame_->extended_data[0]);
                short *dst = reinterpret_cast<short*>(audioBuffer_);
                for (uint32_t i = 0; i < numSamples; i++) {
                    float v = src[i] * 32768.0f;
                    if (v > 32767.0f) v = 32767.0f;
                    if (v < -32768.0f) v = 32768.0f;
                    dst[i] = static_cast<short>(v);
                }

                alSourceStop(alSource_);

                alBufferData(alBuffer, static_cast<ALenum>(alFormat_), audioBuffer_, static_cast<ALsizei>(dataSize), static_cast<ALsizei>(anSampleRate_));
                Audio::PrintError();
                alSourceQueueBuffers(alSource_, 1, &alBuffer);
                Audio::PrintError();

                alSourcePlay(alSource_);

                alNumFreeBuffers_--;
                alFreeBuffers_[alNumFreeBuffers_] = 0;
            }
        }
    }
}
