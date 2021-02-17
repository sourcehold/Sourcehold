#include "AudioEngine.h"
#include <al.h>
#include <stdexcept>
#include "System/Logger.h"

static const char* getError() {
  auto err = alGetError();
  switch (err) {
    case ALC_NO_ERROR:
      return "ALC_NO_ERROR";
    case ALC_INVALID_DEVICE:
      return "ALC_INVALID_DEVICE";
    case ALC_INVALID_CONTEXT:
      return "ALC_INVALID_CONTEXT";
    case ALC_INVALID_ENUM:
      return "ALC_INVALID_ENUM";
    case ALC_INVALID_VALUE:
      return "ALC_INVALID_VALUE";
    default:
      return "UNKNOWN ALC ERROR";
  }
}

void AudioEngine::InitOpenAL() {
  device_ = alcOpenDevice(nullptr);
  if (device_ == nullptr) {
    throw std::runtime_error(getError());
  }
  context_ = alcCreateContext(device_, nullptr);
  if (context_ == nullptr) {
    throw std::runtime_error(getError());
  }
}

AudioEngine::AudioEngine() {
  using namespace Sourcehold::System;
  Logger::message(AUDIO) << "Creating Audio Engine .. " << std::endl;
  try {
    InitOpenAL();
  } catch (std::exception& e) {
    Logger::error(AUDIO) << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

AudioEngine::~AudioEngine() {
  device_ = alcGetContextsDevice(context_);
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(context_);
  alcCloseDevice(device_);
}
