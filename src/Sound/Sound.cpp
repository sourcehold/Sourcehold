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
    PrintError();
    alSourcef(song.source, AL_PITCH, 1);
    PrintError();
    alSourcef(song.source, AL_GAIN, 1);
    PrintError();
    alSource3f(song.source, AL_POSITION, 0, 0, 0);
    PrintError();
    alSource3f(song.source, AL_VELOCITY, 0, 0, 0);
    PrintError();
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

    song.valid = true;
    return song;
}

AudioSource Sound::LoadEffect(std::string path, bool repeat) {
    AudioSource song;

    /* Parameters */
    song.repeat = repeat;
    alGenSources((ALuint)1, &song.source);
    PrintError();
    alSourcef(song.source, AL_PITCH, 1);
    PrintError();
    alSourcef(song.source, AL_GAIN, 1);
    PrintError();
    alSource3f(song.source, AL_POSITION, 0, 0, 0);
    PrintError();
    alSource3f(song.source, AL_VELOCITY, 0, 0, 0);
    PrintError();
    alSourcei(song.source, AL_LOOPING, repeat ? AL_TRUE : AL_FALSE);

    alGenBuffers((ALuint)1, &song.buffer);
    PrintError();

    return song;
}

void Sound::PlayAudio(AudioSource &source) {
    if(!source.valid) return;
}

bool Sound::IsPlaying() {
    return false;
}

void Sound::PrintError() {
    ALCenum err = alGetError();
    if(err == AL_NO_ERROR) return;

    Logger::error("SOUND") << "Error code: " << err << std::endl;
}
