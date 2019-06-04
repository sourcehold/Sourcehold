#include <Audio/AudioSource.h>
#include <Audio/Audio.h>

#include <SDL.h>

using namespace Sourcehold::Audio;
using namespace Sourcehold::System;

AudioSource::AudioSource(const AudioSource &source) {
    this->source = source.source;
    this->buffer = source.buffer;
    this->ptr = source.ptr;
    this->size = source.size;
    this->repeat = source.repeat;
    this->valid = source.valid;
    this->fading = source.fading;
}

AudioSource::AudioSource(boost::filesystem::path path, bool repeat) :
    repeat(repeat),
    valid(false)
{
    /* Default to loading a song */
    if(!LoadSong(path, repeat)) {
        Logger::error("AUDIO") << "AudioSource::AudioSource() argument 'path' resulted in an error!" << std::endl;
    }
}

AudioSource::AudioSource(uint8_t *ptr, size_t size, bool repeat) :
    ptr(ptr),
    size(size),
    repeat(repeat)
{ }

AudioSource::~AudioSource() {
	Destroy();
}

bool AudioSource::LoadSong(boost::filesystem::path path, bool repeat) {
    /* Parameters */
    this->repeat = repeat;
    this->gain = 1.0f;
    this->path = path;
	this->alFormat = AL_FORMAT_MONO16;
	this->alSampleRate = SAMPLING_RATE;
    alGenSources((ALuint)1, &source);
    alSourcef(source, AL_PITCH, 1.0f);
    alSourcef(source, AL_GAIN, IsOpenALMuted() ? 0.0f : gain);
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(source, AL_LOOPING, repeat ? AL_TRUE : AL_FALSE);

    alGenBuffers((ALuint)1, &buffer);
    Audio::PrintError();

    /* Read raw file into buffer */
    FILE *fp = std::fopen(path.string().c_str(), "rb");
    if(!fp) {
        Logger::error("AUDIO") << "Unable to open song '" << path << "'!" << std::endl;
        return false;
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    ptr = (uint8_t*)std::malloc(size);
    fread(ptr, size, 1, fp);
    fclose(fp);

    alBufferData(buffer, alFormat, (const ALvoid*)ptr, size, alSampleRate);
    Audio::PrintError();
    alSourcei(source, AL_BUFFER, buffer);
    Audio::PrintError();

    mode = MODE_PCM;

    valid = true;
    return true;
}

bool AudioSource::LoadEffect(boost::filesystem::path path, bool repeat) {
    /* Parameters */
    this->repeat = repeat;
    this->gain = 1.0f;
    this->path = path;
    alGenSources((ALuint)1, &source);
    alSourcef(source, AL_PITCH, 1.0f);
    alSourcef(source, AL_GAIN, IsOpenALMuted() ? 0.0f : gain);
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(source, AL_LOOPING, repeat ? AL_TRUE : AL_FALSE);

    alGenBuffers((ALuint)1, &buffer);
    Audio::PrintError();

	if (!InitFFmpeg()) {
		Logger::error("AUDIO") << "AudioSource::LoadEffect(): Error while initializing FFmpeg!" << std::endl;
		alDeleteSources(1, &source);
		Audio::PrintError();
		alDeleteBuffers(1, &buffer);
		Audio::PrintError();
		return false;
	}

	mode = MODE_ADPCM;

    valid = true;
    return true;
}

bool AudioSource::Play() {
	if (mode == MODE_ADPCM) {

	}
	else {
		alSourceRewind(source);
		Audio::PrintError();
		alSourcePlay(source);
		Audio::PrintError();
	}

    return true;
}

void AudioSource::Pause() {
	if (mode == MODE_ADPCM) {

	}
	else {
		alSourcePause(source);
		Audio::PrintError();
	}
}

void AudioSource::Resume() {
	if (mode == MODE_ADPCM) {

	}
	else {
		alSourcePlay(source);
		Audio::PrintError();
	}
}

void AudioSource::Stop() {
	if (mode == MODE_ADPCM) {

	}
	else {
		alSourceStop(source);
		Audio::PrintError();
	}
}

void AudioSource::Rewind() {
	if (mode == MODE_ADPCM) {
		av_seek_frame(ic, -1, 0, 0);
	}
	else {
		alSourceRewind(source);
		Audio::PrintError();
	}
}

void AudioSource::SetFadeOut(double amount) {
    if(fading) return;
    fadeAmount = amount;
    fading = true;
    fadeBase = (double)SDL_GetTicks() / 1000.0;
    fadeIn = false;
}

void AudioSource::SetFadeIn(double amount) {
    if(fading) return;
    fadeAmount = amount;
    fading = true;
    fadeBase = (double)SDL_GetTicks() / 1000.0;
    fadeIn = true;
}

void AudioSource::UpdateFade() {
    if(!fading) return;

    if(fadeIn) {
	    double t = ((double)SDL_GetTicks() / 1000.0) - fadeBase;
	    if(t > fadeAmount) {
	        Pause();
	        fading = false;
	        gain = 1.0;
	    }else {
	        alSourcef(source, AL_GAIN, IsOpenALMuted() ? 0.0f : gain * (t / fadeAmount));
	    }
    }else {
	    double t = ((double)SDL_GetTicks() / 1000.0) - fadeBase;
	    if(t > fadeAmount) {
	        Pause();
	        fading = false;
	        gain = 1.0;
	    }else {
	        alSourcef(source, AL_GAIN, IsOpenALMuted() ? 0.0f : gain * (1.0 - t / fadeAmount));
	    }
    }
}

void AudioSource::Destroy() {
	if (valid) {
		if (mode == MODE_ADPCM) {
			DestroyFFmpeg();
		}

		alDeleteSources(1, &source);
		Audio::PrintError();
		alDeleteBuffers(1, &buffer);
		Audio::PrintError();

		if (ffmpegRunning || mode == MODE_PCM) {
			free(ptr);
		}

		valid = false;
	}
}

void AudioSource::Update() {
	if (mode == MODE_ADPCM && ffmpegRunning) {
		if (delayTimer > 0) {
			delayTimer -= SDL_GetTicks();
			return;
		}

		av_init_packet(&audioPacket);

		int ret;
		if (av_read_frame(ic, &audioPacket) < 0) {
			if (repeat) {
				Rewind();
				if (av_read_frame(ic, &audioPacket) < 0) {
					return;
				}
			}
			else {
				ffmpegRunning = false;
				return;
			}
		}

		if (audioPacket.stream_index == audioStream && !IsOpenALMuted()) {
			av_frame_unref(audioFrame);

			ret = avcodec_send_packet(audioCtx, &audioPacket);
			if (ret < 0) return;

			while (ret >= 0 && delayTimer <= 0) {
				ret = avcodec_receive_frame(audioCtx, audioFrame);
				if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
					return;
				}

				delayTimer = (int)((double)audioFrame->pts * av_q2d(ic->streams[audioStream]->time_base) * 1000.0);

				if (!ffmpegRunning) {
					alGenSources((ALuint)1, &source);
					alSourcef(source, AL_PITCH, 1.0f);
					alSourcef(source, AL_GAIN, IsOpenALMuted() ? 0.0f : gain);
					alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
					alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
					alSourcei(source, AL_LOOPING, repeat ? AL_TRUE : AL_FALSE);
					alGenBuffers(NUM_AUDIO_BUFFERS, alBuffers);
					Audio::PrintError();

					/* Determine number of channels and format */
					audioFrame->channel_layout = av_get_default_channel_layout(audioCtx->channels);
					if (audioFrame->channel_layout == AV_CH_LAYOUT_MONO) {
						alFormat = (audioFrame->format == AV_SAMPLE_FMT_U8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
						alNumChannels = 1;
					}
					else if (audioFrame->channel_layout == AV_CH_LAYOUT_STEREO) {
						alFormat = (audioFrame->format == AV_SAMPLE_FMT_U8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
						alNumChannels = 2;
					}
					else {
						std::cout << audioFrame->channel_layout << std::endl;
						return;
					}

					/* Setup audio queue */
					alNumFreeBuffers = NUM_AUDIO_BUFFERS;
					for (int i = 0; i < NUM_AUDIO_BUFFERS; i++) {
						alFreeBuffers[i] = alBuffers[i];
					}

					alSampleRate = audioFrame->sample_rate;
					size = av_samples_get_buffer_size(
						NULL,
						alNumChannels,
						audioFrame->nb_samples,
						(AVSampleFormat)audioFrame->channel_layout,
						1
					);
					ptr = (uint8_t*)std::malloc(size);

					ffmpegRunning = true;
				}
				else {
					std::memset(ptr, 0, alNumChannels * audioFrame->nb_samples * 4);

					int buffersFinished = 0;
					alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersFinished);
					Audio::PrintError();

					if (buffersFinished > 0) {
						alSourceStop(source);

						for (; buffersFinished > 0; buffersFinished--) {
							ALuint buffer = 0;
							alSourceUnqueueBuffers(source, 1, &buffer);
							Audio::PrintError();

							if (buffer > 0) {
								alFreeBuffers[alNumFreeBuffers] = buffer;
								Audio::PrintError();
								alNumFreeBuffers++;
							}
						}

						alSourcePlay(source);
					}

					if (alNumFreeBuffers > 0) {
						/**
						* TODO: Replace with libswresample
						* Kept crashing on me though :O
						*/
						if (audioFrame->format != AV_SAMPLE_FMT_S16) {
							return;
						}

						ALuint alBuffer = alFreeBuffers[alNumFreeBuffers - 1];
						uint32_t numSamples = audioFrame->nb_samples * alNumChannels;
						uint32_t dataSize = numSamples * 2;

						/* Convert samples */
						unsigned short* src = (unsigned short*)audioFrame->extended_data[0];
						int16_t *dst = (int16_t*)ptr;
						for (int i = 0; i < numSamples; i++)
							dst[i] = src[i];

						alSourceStop(source);

						alBufferData(alBuffer, alFormat, ptr, size, alSampleRate);
						Audio::PrintError();
						alSourceQueueBuffers(source, 1, &alBuffer);
						Audio::PrintError();

						alSourcePlay(source);

						alNumFreeBuffers--;
						alFreeBuffers[alNumFreeBuffers] = 0;
					}
				}
			}
		}
	}
}

bool AudioSource::IsValid() {
    return valid;
}

bool AudioSource::IsPlaying() {
	bool playing = true;
	if (ffmpegInited) {

	}
	else {
		ALenum state;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		playing = state == AL_PLAYING;
	}

	return playing;
}

bool AudioSource::IsRepeating() {
    return repeat;
}

bool AudioSource::InitFFmpeg() {
	if (ffmpegInited) return true;

	ic = avformat_alloc_context();
	if (!ic) {
		return false;
	}

	int out = avformat_open_input(
		&ic,
		path.string().c_str(),
		Game::GetAVInputFormat(),
		NULL
	);
	if (out < 0) {
		return false;
	}

	ic->max_analyze_duration = 10000;
	if (avformat_find_stream_info(ic, NULL) < 0) {
		return false;
	};

	audioStream = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, &audioDecoder, 0);
	if (audioStream < 0) {
		return false;
	}

	audioCtx = avcodec_alloc_context3(audioDecoder);
	if (!audioCtx) {
		return false;
	}

	avcodec_parameters_to_context(audioCtx, ic->streams[audioStream]->codecpar);
	int ca = avcodec_open2(audioCtx, audioDecoder, NULL);
	if (ca < 0) {
		return false;
	}

	audioFrame = av_frame_alloc();
	if (!audioFrame) {
		return false;
	}

	delayTimer = 0;
	ffmpegInited = true;

	return true;
}

void AudioSource::DestroyFFmpeg() {
	if (!ffmpegInited) return;

	avformat_close_input(&ic);
	av_frame_free(&audioFrame);
	audioDecoder->close(audioCtx);
	av_free(audioCtx);

	alSourceStop(source);
	alDeleteBuffers(NUM_AUDIO_BUFFERS, alBuffers);

	ffmpegInited = false;
}
