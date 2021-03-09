#include "Audio/Song.h"
#include "Audio/Audio.h"

#include <SDL.h>

using namespace Sourcehold::Audio;
using namespace Sourcehold::System;


Song::Song(ghc::filesystem::path path, bool repeat) :
    repeat_(repeat),
    valid_(false)
{
    if(!Load(path, repeat)) {
        Logger::error(AUDIO) << "Song::Song() argument 'path' resulted in an error!" << std::endl;
    }
}

Song::Song(uint8_t *ptr, size_t size, bool repeat) :
    ptr_(ptr),
    size_(size),
    repeat_(repeat)
{ }

Song::~Song()
{
    Destroy();
}

bool Song::Load(ghc::filesystem::path path, bool repeat)
{
    /* Parameters */
    repeat_ = repeat;
    gain_ = 1.0f;
    alGenSources(1, &source_);
    alSourcef(source_, AL_PITCH, 1.0f);
    alSourcef(source_, AL_GAIN, IsOpenALMuted() ? 0.0f : gain_);
    alSource3f(source_, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(source_, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(source_, AL_LOOPING, repeat ? AL_TRUE : AL_FALSE);

    alGenBuffers(1, &buffer_);
    Audio::PrintError();

    /* Read raw file into buffer */
    FILE *fp = std::fopen(path.string().c_str(), "rb");
    if(!fp) {
        Logger::error(AUDIO) << "Unable to open song '" << path << "'!" << std::endl;
        return false;
    }
    fseek(fp, 0, SEEK_END);
    size_ = static_cast<size_t>(ftell(fp));
    fseek(fp, 0, SEEK_SET);

    ptr_ = static_cast<uint8_t*>(std::malloc(size_));
    fread(ptr_, size_, 1, fp);
    fclose(fp);

    alBufferData(buffer_, AL_FORMAT_MONO16, static_cast<const ALvoid*>(ptr_), static_cast<ALsizei>(size_), sampling_rate_);
    Audio::PrintError();
    alSourcei(source_, AL_BUFFER, static_cast<ALint>(buffer_));
    Audio::PrintError();

    valid_ = true;
    return true;
}

bool Song::Play()
{
    alSourceRewind(source_);
    Audio::PrintError();
    alSourcePlay(source_);
    Audio::PrintError();

    return true;
}

void Song::Pause()
{
    alSourcePause(source_);
    Audio::PrintError();
}

void Song::Resume()
{
    alSourcePlay(source_);
    Audio::PrintError();
}

void Song::Stop()
{
    alSourceStop(source_);
    Audio::PrintError();
}

void Song::Rewind()
{
    alSourceRewind(source_);
    Audio::PrintError();
}

void Song::SetFadeOut(double amount)
{
    if(fading_) return;
    fade_amount_ = amount;
    fading_ = true;
    fade_base_ = SDL_GetTicks() / 1000.0;
    fade_in_ = false;
}

void Song::SetFadeIn(double amount)
{
    if(fading_) return;
    fade_amount_ = amount;
    fading_ = true;
    fade_base_ = SDL_GetTicks() / 1000.0;
    fade_in_ = true;
}

void Song::UpdateFade()
{
    if(!fading_) return;

    if(fade_in_) {
        auto t = SDL_GetTicks() / 1000.0 - fade_base_;
        if(t > fade_amount_) {
            Pause();
            fading_ = false;
            gain_ = 1.0;
        }
        else {
            alSourcef(source_, AL_GAIN, IsOpenALMuted() ? 0.0f : static_cast<float>(static_cast<double>(gain_) * (t / fade_amount_)));
        }
    }
    else {
        auto t = SDL_GetTicks() / 1000.0 - fade_base_;
        if(t > fade_amount_) {
            Pause();
            fading_ = false;
            gain_ = 1.0;
        }
        else {
            alSourcef(source_, AL_GAIN, IsOpenALMuted() ? 0.0f : static_cast<float>(static_cast<double>(gain_) * (1.0 - t / fade_amount_)));
        }
    }
}

void Song::Destroy()
{
    if (valid_) {
        alDeleteSources(1, &source_);
        //Audio::PrintError();
        alDeleteBuffers(1, &buffer_);
        //Audio::PrintError();

        free(ptr_);

        valid_ = false;
    }
}

bool Song::IsPlaying()
{
    ALenum state;
    alGetSourcei(source_, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

