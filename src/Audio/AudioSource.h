#pragma once

#include <cinttypes>
#include <string>
#include <cstdio>

#include <al.h>
#include <alc.h>

#include <boost/filesystem.hpp>

#include <FFmpegUtil.h>
#include <System/Logger.h>

namespace Sourcehold
{
    namespace Audio
    {
        /**
         * A single audio source, assumes that the audio context
         * has been created already. This handles two use cases:
		 * - Raw PCM audio (16-bit MONO) at 44100 baud used for songs
		 * - WAV ADPCM audio used for SFX
		 * This class can be reused by calling Destroy() and initializing
		 * a new effect/song afterwards.
         */
        class AudioSource
        {
            boost::filesystem::path path;
			enum DecoderMode {
				MODE_PCM,
				MODE_ADPCM
			} mode;
			/* OpenAL stuff */
			const static uint32_t NUM_AUDIO_BUFFERS = 4;
			ALuint alFreeBuffers[NUM_AUDIO_BUFFERS];
			ALuint alBuffers[NUM_AUDIO_BUFFERS];
			ALuint alNumFreeBuffers = NUM_AUDIO_BUFFERS;
			ALuint alNumChannels;
			ALuint alFormat;
			ALuint alSampleRate;
            ALuint source;
            ALuint buffer;
            uint8_t *ptr;
            size_t size;
			/* FFmpeg stuff */
			AVFormatContext *ic;
			AVCodecContext *audioCtx;
			AVCodec *audioDecoder;
			AVPacket audioPacket;
			AVFrame *audioFrame;
			int audioStream;
			int delayTimer;
			/* Properties */
			bool repeat, valid = false, fading = false;
			bool ffmpegInited = false, ffmpegRunning = false;
            float gain; /* Current gain */
            double fadeBase = 0.0, fadeAmount = 0.0;
            const uint32_t SAMPLING_RATE = 44100;
        public:
            AudioSource(const AudioSource &source);
            AudioSource(boost::filesystem::path path, bool repeat = false);
            AudioSource(uint8_t *ptr = NULL, size_t size = 0, bool repeat = false);
            ~AudioSource();

            bool LoadSong(boost::filesystem::path path, bool repeat = false);
            bool LoadEffect(boost::filesystem::path path, bool repeat = false);
            bool Play();
            void Pause();
            void Resume();
            void Stop();
			void Rewind();
            void SetFadeOut(double amount = 1.0);
            void UpdateFade();
			void Destroy();
			void Update();

            bool IsValid();
            bool IsPlaying();
            bool IsRepeating();
        protected:
			bool InitOpenAL();
			void DestroyOpenAL();
        };
    }
}
