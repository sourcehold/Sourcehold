#include <cstring>

#include <SDL.h>

#include <Rendering/BinkVideo.h>
#include <Audio/Audio.h>
#include <FFmpegUtil.h>

using namespace Sourcehold;
using namespace System;
using namespace Audio;
using namespace Rendering;

static AVCodec *bink_codec;

BinkVideo::BinkVideo(std::shared_ptr<Renderer> man) : Texture(man)
{
    ic = avformat_alloc_context();
    if(!ic) {
        Logger::error("RENDERING") << "Unable to allocate input format context!" << std::endl;
    }
}

BinkVideo::BinkVideo(std::shared_ptr<Renderer> man, boost::filesystem::path path, bool looping) : Texture(man) {
    ic = avformat_alloc_context();
    if(!ic) {
        Logger::error("RENDERING") << "Unable to allocate input format context!" << std::endl;
    }

    LoadFromDisk(path, looping);
}

BinkVideo::~BinkVideo() {
    Close();
}

bool BinkVideo::LoadFromDisk(boost::filesystem::path path, bool looping) {
    this->looping = looping;

    int out = avformat_open_input(
        &ic,
        path.string().c_str(),
		Game::GetAVInputFormat(),
        NULL
    );
    if(out < 0) {
        return false;
    }

    ic->max_analyze_duration = 10000;
    if(avformat_find_stream_info(ic, NULL) < 0) {
        return false;
    };

    videoStream = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);
    audioStream = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, &audioDecoder, 0);
    if(videoStream < 0) {
        return false;
    }

    fps = (float)ic->streams[videoStream]->avg_frame_rate.num / (float)ic->streams[videoStream]->avg_frame_rate.den;

    if(audioStream >= 0) {
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

        hasAudio = true;
    }

    codecCtx = avcodec_alloc_context3(decoder);
    if(!codecCtx) {
        return false;
    }

    avcodec_parameters_to_context(codecCtx, ic->streams[videoStream]->codecpar);

    uint8_t bink_extradata[4] = { 0 } ;
    codecCtx->extradata = bink_extradata;
    codecCtx->extradata_size = sizeof(bink_extradata);

    int cv = avcodec_open2(codecCtx, decoder, NULL);
    if(cv < 0) {
        return false;
    }

    frame = av_frame_alloc();
    if(!frame) {
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
        return false;
    }

    Texture::AllocNewStreaming(800, 600, SDL_PIXELFORMAT_RGB888);
	framebuf = (uint32_t*)malloc(800 * 600 * sizeof(uint32_t));

    valid = true;
    running = true;

    lastTicks = SDL_GetTicks();
    return true;
}

void BinkVideo::Close() {
    if(valid) {
        avformat_close_input(&ic);
        av_frame_free(&frame);
        decoder->close(codecCtx);
        av_free(codecCtx);
		free(framebuf);

        if(hasAudio) {
            decoder->close(audioCtx);
            av_free(audioCtx);
            av_frame_free(&audioFrame);
            alSourceStop(alSource);
            alDeleteSources(1, &alSource);
            alDeleteBuffers(NUM_AUDIO_BUFFERS, alBuffers);
        }
    }
}

void BinkVideo::Update() {
    if(!running || !valid) return;

	av_init_packet(&packet);

	int ret;
	if (av_read_frame(ic, &packet) < 0) {
		if (looping) {
			av_seek_frame(ic, -1, 0, 0);
			if (av_read_frame(ic, &packet) < 0) {
				return;
			}
		}
		else {
			running = false;
			return;
		}
	}

	if (packet.stream_index == videoStream) {
		if (avcodec_send_packet(codecCtx, &packet) < 0) return;

		ret = avcodec_receive_frame(codecCtx, frame);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			return;
		}

		uint8_t* slices[3] = { (uint8_t*)&framebuf[0], 0, 0 };
		int strides[3] = { 800 * 4, 0, 0 };

		Texture::LockTexture();

		sws_scale(sws, frame->data, frame->linesize, 0, codecCtx->height, slices, strides);
		std::memcpy(Texture::GetData(), framebuf, 800 * 600 * 4);

		Texture::UnlockTexture();
	}
	else if (packet.stream_index == audioStream && !IsOpenALMuted()) {
		av_frame_unref(audioFrame);

		ret = avcodec_send_packet(audioCtx, &packet);
		if (ret < 0) return;

		while (ret >= 0) {
			ret = avcodec_receive_frame(audioCtx, audioFrame);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
				return;
			}

			if (!audioInit) {
				/* Init OpenAL stuff */
				alGenSources(1, &alSource);
				Audio::PrintError();
				alGenBuffers(NUM_AUDIO_BUFFERS, alBuffers);
				Audio::PrintError();

				alSource3f(alSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
				alSource3f(alSource, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
				alSourcef(alSource, AL_PITCH, 1.0f);
				alSourcef(alSource, AL_GAIN, 1.0f);

				/* Determine number of channels and format */
				if (audioFrame->channel_layout == AV_CH_LAYOUT_MONO) {
					alFormat = (audioFrame->format == AV_SAMPLE_FMT_U8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
					alNumChannels = 1;
				}
				else if (audioFrame->channel_layout == AV_CH_LAYOUT_STEREO) {
					alFormat = (audioFrame->format == AV_SAMPLE_FMT_U8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
					alNumChannels = 2;
				}
				else {
					Logger::error("RENDERING") << "Bink audio channel layout is wrong!" << std::endl;
					return;
				}

				/* Setup audio queue */
				alNumFreeBuffers = NUM_AUDIO_BUFFERS;
				for (int i = 0; i < NUM_AUDIO_BUFFERS; i++) {
					alFreeBuffers[i] = alBuffers[i];
				}

				alSampleRate = audioFrame->sample_rate;
				size = alNumChannels * audioFrame->nb_samples * 4;
				audioBuffer = (char*)std::malloc(size);

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
				/**
				* TODO: Other versions of Stronghold might include
				* different audio formats (investigate!)
				*/
				if (audioFrame->format != AV_SAMPLE_FMT_FLT) {
					return;
				}

				ALuint alBuffer = alFreeBuffers[alNumFreeBuffers - 1];
				uint32_t numSamples = audioFrame->nb_samples * alNumChannels;
				uint32_t dataSize = numSamples * 2;

				/* Convert samples */
				float *src = (float*)audioFrame->extended_data[0];
				short *dst = (short*)audioBuffer;
				for (int i = 0; i < numSamples; i++) {
					float v = src[i] * 32768.0f;
					if (v > 32767.0f) v = 32767.0f;
					if (v < -32768.0f) v = 32768.0f;
					dst[i] = (short)v;
				}

				alSourceStop(alSource);

				alBufferData(alBuffer, alFormat, audioBuffer, dataSize, alSampleRate);
				Audio::PrintError();
				alSourceQueueBuffers(alSource, 1, &alBuffer);
				Audio::PrintError();

				alSourcePlay(alSource);
				
				alNumFreeBuffers--;
				alFreeBuffers[alNumFreeBuffers] = 0;
			}
		}
	}
}

