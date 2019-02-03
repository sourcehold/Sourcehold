#include <Sound.h>

using namespace OpenSH::System;
using namespace OpenSH::Sound;

Sound::Sound() {
}

Sound::~Sound() {
    alcCloseDevice(device);
}

bool Sound::Init() {
    device = alcOpenDevice(NULL);
    if(!device) {
        Logger::warning("SOUND") << "Unable to open AL device!" << std::endl;
        return false;
    }

    return true;
}

void Sound::PrintError() {
    ALCenum error = alGetError();
    if(error != AL_NO_ERROR) {
        Logger::error("SOUND") << alutGetErrorString(error) << std::endl;
    }
}
