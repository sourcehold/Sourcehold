#include <Sound/Sound.h>

using namespace Sourcehold::System;
using namespace Sourcehold::Sound;

Sound::Sound() {
}

Sound::Sound(const Sound &snd) {
    device = snd.device;
    context = snd.context;
}

Sound::~Sound() {
    //alcCloseDevice(device);
    //PrintError();
}

bool Sound::Init() {
    alGetError();

    device = alcOpenDevice(NULL);
    if(!device) {
        PrintError();
        return false;
    }

    context = alcCreateContext(device, NULL);
    if(!alcMakeContextCurrent(context)) {
        PrintError();
        return false;
    }

    return true;
}

AudioSource Sound::LoadSong(std::string path, bool repeat) {
    AudioSource song;

    /* Parameters */
    song.valid = false;
    song.repeat = repeat;
    alGenSources((ALuint)1, &song.source);
    alSourcef(song.source, AL_PITCH, 1.0f);
    alSourcef(song.source, AL_GAIN, 1.0f);
    alSource3f(song.source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(song.source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(song.source, AL_LOOPING, repeat ? AL_TRUE : AL_FALSE);

    alGenBuffers((ALuint)1, &song.buffer);
    PrintError();

    /* Read raw file into buffer */
    FILE *fp = std::fopen(path.c_str(), "rb");
    if(!fp) {
        Logger::error("SOUND") << "Unable to open song '" << path << "'!" << std::endl;
        return song;
    }
    fseek(fp, 0, SEEK_END);
    song.size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    song.ptr = (uint8_t*)std::malloc(song.size);
    fread(song.ptr, song.size, 1, fp);
    fclose(fp);

    alBufferData(song.buffer, AL_FORMAT_MONO16, (const ALvoid*)song.ptr, song.size, 44100);
    alSourcei(song.source, AL_BUFFER, song.buffer);

    song.valid = true;
    return song;
}

AudioSource Sound::LoadEffect(std::string path, bool repeat) {
    AudioSource song;

    /* Parameters */
    song.repeat = repeat;
    alGenSources((ALuint)1, &song.source);
    alSourcef(song.source, AL_PITCH, 1.0f);
    alSourcef(song.source, AL_GAIN, 1.0f);
    alSource3f(song.source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(song.source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(song.source, AL_LOOPING, repeat ? AL_TRUE : AL_FALSE);

    alGenBuffers((ALuint)1, &song.buffer);
    PrintError();

    return song;
}

void Sound::PlayAudio(AudioSource &source) {
    if(!source.valid) return;

    alSourcePlay(source.source);
    PrintError();
}

bool Sound::IsPlaying() {
    return false;
}

void Sound::PrintError() {
    ALCenum err = alGetError();
    std::string str;

    if(err == AL_NO_ERROR) return;
    switch (err) {
        case ALC_NO_ERROR: str = "AL_NO_ERROR"; break;
        case ALC_INVALID_DEVICE: str = "AL_INVALID_DEVICE"; break;
        case ALC_INVALID_CONTEXT: str = "AL_INVALID_CONTEXT"; break;
        case ALC_INVALID_ENUM: str = "AL_INVALID_ENUM"; break;
        case ALC_INVALID_VALUE: str = "AL_INVALID_VALUE"; break;
        case ALC_OUT_OF_MEMORY: str = "AL_OUT_OF_MEMORY"; break;
        default: str = "Unknown error"; break;
    }

    Logger::error("SOUND") << str << std::endl;
}
