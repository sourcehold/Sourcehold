#include <Audio/AudioSource.h>
#include <Audio/Audio.h>

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

AudioSource::AudioSource(const std::string &path, bool repeat) :
    repeat(repeat),
    valid(false)
{
    /* Default to loading a song */
    if(!LoadSong(path, repeat)) {
        Logger::error("AUDIO") << "AudioSource::AudioSource() argument 'path' resulted in an error!" << std::endl;
    }
}

AudioSource::AudioSource(uint8_t *ptr, size_t size, bool repeat) :
    repeat(repeat),
    size(size),
    ptr(ptr),
    valid(false)
{ }

AudioSource::~AudioSource() {
    alDeleteSources(1, &source);
    Audio::PrintError();
    alDeleteBuffers(1, &buffer);
    Audio::PrintError();
}

bool AudioSource::LoadSong(const std::string &path, bool repeat) {
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
    FILE *fp = std::fopen(path.c_str(), "rb");
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

    alBufferData(buffer, AL_FORMAT_MONO16, (const ALvoid*)ptr, size, SAMPLING_RATE);
    Audio::PrintError();
    alSourcei(source, AL_BUFFER, buffer);
    Audio::PrintError();

    valid = true;
    return true;
}

bool AudioSource::LoadEffect(const std::string &path, bool repeat) {
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

    valid = true;
    return true;
}

bool AudioSource::Create(void *buffer, size_t size, bool repeat) {
    this->repeat = repeat;
    this->ptr = (uint8_t*)buffer;
    this->size = size;

    valid = true;
    return true;
}

bool AudioSource::Play() {
    alSourceRewind(source);
    Audio::PrintError();
    alSourcePlay(source);
    Audio::PrintError();
    return true;
}

void AudioSource::Pause() {
    alSourcePause(source);
    Audio::PrintError();
}

void AudioSource::Resume() {
    alSourcePlay(source);
    Audio::PrintError();
}

void AudioSource::Stop() {
    alSourceStop(source);
    Audio::PrintError();
}

void AudioSource::SetFadeOut(double amount) {
    if(fading) return;
    fadeAmount = amount;
    fading = true;
    fadeBase = (double)SDL_GetTicks() / 1000.0; 
}

void AudioSource::UpdateFade() {
    if(!fading) return;

    double t = ((double)SDL_GetTicks() / 1000.0) - fadeBase;
    if(t > fadeAmount) {
        Stop();
        fading = false;
    }else {
        alSourcef(source, AL_GAIN, IsOpenALMuted() ? 0.0f : gain * (1.0 - t / fadeAmount));
    }
}

bool AudioSource::IsValid() {
    return valid;
}

bool AudioSource::IsPlaying() {
    ALenum state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

bool AudioSource::IsRepeating() {
    return repeat;
}
