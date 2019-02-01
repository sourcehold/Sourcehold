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
        Logger::LogToConsole("Unable to open AL device!", Logger::ERROR);
        return false;
    }



    return true;
}

void Sound::PrintError() {
    ALCenum error = alGetError();
    if(error != AL_NO_ERROR) {
        Logger::LogToConsole(alutGetErrorString(error), Logger::ERROR);
    }
}
