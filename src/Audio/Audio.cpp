#include <string>

#include <al.h>
#include <alc.h>

#include <Audio/Audio.h>
#include <System/Logger.h>

using namespace Sourcehold;
using namespace System;

static ALCdevice *_device;
static ALCcontext *_context;
static bool _muted = false;

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

void Audio::MuteOpenAL() {
    _muted = true;
}

void Audio::UnmuteOpenAL() {
    _muted = false;
}

bool Audio::IsOpenALMuted() {
    return _muted;
}

void Audio::PrintError() {
    ALCenum err = alGetError();
    std::string str;

    if(err == AL_NO_ERROR) return;
    switch (err) {
        case ALC_NO_ERROR: str = "ALC_NO_ERROR"; break;
        case ALC_INVALID_DEVICE: str = "ALC_INVALID_DEVICE"; break;
        case ALC_INVALID_CONTEXT: str = "ALC_INVALID_CONTEXT"; break;
        case ALC_INVALID_ENUM: str = "ALC_INVALID_ENUM"; break;
        case ALC_INVALID_VALUE: str = "ALC_INVALID_VALUE"; break;
        case ALC_OUT_OF_MEMORY: str = "ALC_OUT_OF_MEMORY"; break;
        default: str = "Unknown error"; break;
    }

    Logger::error("AUDIO") << str << std::endl;
}
