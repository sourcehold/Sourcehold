#include <string>

#include <Audio/Audio.h>
#include <System/Logger.h>

using namespace Sourcehold;
using namespace System;

static ALCdevice *_device;
static ALCcontext *_context;

void Audio::PrintError() {
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

bool Audio::InitOpenAL() {
    _device = alcOpenDevice(NULL);
    if(!_device) {
        PrintError();
        return false;
    }

    _context = alcCreateContext(_device, NULL);
    if(!alcMakeContextCurrent(_context)) {
        PrintError();
        return false;
    }

    return true;
}

void Audio::DestroyOpenAL() {
    _device = alcGetContextsDevice(_context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(_context);
    alcCloseDevice(_device);
}
