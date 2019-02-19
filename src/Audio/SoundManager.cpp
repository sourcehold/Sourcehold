#include <Audio/SoundManager.h>

using namespace Sourcehold::System;
using namespace Sourcehold::Audio;

SoundManager::SoundManager() {
}

SoundManager::SoundManager(const SoundManager &snd) {
    device = snd.device;
    context = snd.context;
}

SoundManager::~SoundManager() {
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

bool SoundManager::Init() {
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

void SoundManager::PrintError() {
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

    Logger::error("AUDIO") << str << std::endl;
}
