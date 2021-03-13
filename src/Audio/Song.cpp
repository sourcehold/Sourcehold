#include "Audio/Song.h"
#include "Audio/Audio.h"

#include <SDL.h>

using namespace Sourcehold::Audio;
using namespace Sourcehold::System;

Song::Song(const Song &source)
{
    this->source = source.source;
    this->buffer = source.buffer;
    this->ptr = source.ptr;
    this->size = source.size;
    this->repeat = source.repeat;
    this->valid = source.valid;
    this->fading = source.fading;
}

Song::Song(ghc::filesystem::path path, bool repeat) :
    repeat(repeat),
    valid(false)
{
    if(!Load(path, repeat)) {
        Logger::error(AUDIO) << "Song::Song() argument 'path' resulted in an error!" << std::endl;
    }
}

Song::Song(uint8_t *ptr, size_t size, bool repeat) :
    ptr(ptr),
    size(size),
    repeat(repeat)
{ }

Song::~Song()
{
    Destroy();
}

bool Song::Load(ghc::filesystem::path path, bool repeat)
{
    /* Parameters */
    this->repeat = repeat;
    this->gain = 1.0f;
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
        Logger::error(AUDIO) << "Unable to open song '" << path << "'!" << std::endl;
        return false;
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    ptr = (uint8_t*)std::malloc(size);
    fread(ptr, size, 1, fp);
    fclose(fp);

    alBufferData(buffer, AL_FORMAT_MONO16, static_cast<const ALvoid*>(ptr), static_cast<ALsizei>(size), SAMPLING_RATE);
    Audio::PrintError();
    alSourcei(source, AL_BUFFER, buffer);
    Audio::PrintError();

    valid = true;
    return true;
}

bool Song::Play()
{
    alSourceRewind(source);
    Audio::PrintError();
    alSourcePlay(source);
    Audio::PrintError();

    return true;
}

void Song::Pause()
{
    alSourcePause(source);
    Audio::PrintError();
}

void Song::Resume()
{
    alSourcePlay(source);
    Audio::PrintError();
}

void Song::Stop()
{
    alSourceStop(source);
    Audio::PrintError();
}

void Song::Rewind()
{
    alSourceRewind(source);
    Audio::PrintError();
}

void Song::SetFadeOut(double amount)
{
    if(fading) return;
    fadeAmount = amount;
    fading = true;
    fadeBase = (double)SDL_GetTicks() / 1000.0;
    fadeIn = false;
}

void Song::SetFadeIn(double amount)
{
    if(fading) return;
    fadeAmount = amount;
    fading = true;
    fadeBase = (double)SDL_GetTicks() / 1000.0;
    fadeIn = true;
}

void Song::UpdateFade()
{
    if(!fading) return;

    if(fadeIn) {
        double t = ((double)SDL_GetTicks() / 1000.0) - fadeBase;
        if(t > fadeAmount) {
            Pause();
            fading = false;
            gain = 1.0;
        }
        else {
            alSourcef(source, AL_GAIN, IsOpenALMuted() ? 0.0f : gain * (t / fadeAmount));
        }
    }
    else {
        double t = ((double)SDL_GetTicks() / 1000.0) - fadeBase;
        if(t > fadeAmount) {
            Pause();
            fading = false;
            gain = 1.0;
        }
        else {
            alSourcef(source, AL_GAIN, IsOpenALMuted() ? 0.0f : gain * (1.0 - t / fadeAmount));
        }
    }
}

void Song::Destroy()
{
    if (valid) {
        alDeleteSources(1, &source);
        //Audio::PrintError();
        alDeleteBuffers(1, &buffer);
        //Audio::PrintError();

        free(ptr);

        valid = false;
    }
}

bool Song::IsPlaying()
{
    ALenum state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

