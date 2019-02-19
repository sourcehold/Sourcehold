#include <Audio/AudioSource.h>

using namespace Sourcehold::Audio;
using namespace Sourcehold::System;

AudioSource::AudioSource(const AudioSource &source) :
    source(source.source),
    buffer(source.buffer),
    ptr(source.ptr),
    size(source.size),
    repeat(source.repeat),
    valid(source.valid)
{ }

AudioSource::AudioSource(const std::string path, bool repeat) :
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
    //alDeleteSources(1, &source);
    //alDeleteBuffers(1, &buffer);
}

bool AudioSource::LoadSong(const std::string path, bool repeat) {
    /* Parameters */
    this->repeat = repeat;
    alGenSources((ALuint)1, &source);
    alSourcef(source, AL_PITCH, 1.0f);
    alSourcef(source, AL_GAIN, 1.0f);
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(source, AL_LOOPING, repeat ? AL_TRUE : AL_FALSE);

    alGenBuffers((ALuint)1, &buffer);

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
    alSourcei(source, AL_BUFFER, buffer);

    valid = true;
    return true;
}

bool AudioSource::LoadEffect(const std::string path, bool repeat) {
    /* Parameters */
    this->repeat = repeat;
    this->gain = 1.0f;
    alGenSources((ALuint)1, &source);
    alSourcef(source, AL_PITCH, 1.0f);
    alSourcef(source, AL_GAIN, gain);
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(source, AL_LOOPING, repeat ? AL_TRUE : AL_FALSE);

    alGenBuffers((ALuint)1, &buffer);

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
    alSourcePlay(source);
    return true;
}

void AudioSource::Pause() {
    alSourcePause(source);
}

void AudioSource::Resume() {
    alSourcePlay(source);
}

void AudioSource::Stop() {
    alSourceStop(source);
}

bool AudioSource::IsValid() {
    return valid;
}

bool AudioSource::IsPlaying() {
    ALenum state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);

    if(state != AL_PLAYING) {
        std::cout << state << std::endl;
    }

    return state == AL_PLAYING;
}

bool AudioSource::IsRepeating() {
    return repeat;
}
