#include "OpenAL.h"
#include <stdexcept>

using namespace Sourcehold::OpenAL;
static ALCdevice* al_device_;
static ALCcontext* al_context_;
static bool muted_ = false;

void OpenAL::Init() {
  al_device_ = alcOpenDevice(NULL);
  if (!al_device_) {
    throw std::runtime_error("Failed to init OpenAL device: " + GetError());
  }

  al_context_ = alcCreateContext(al_device_, NULL);
  if (!alcMakeContextCurrent(al_context_)) {
    throw std::runtime_error("Failed to create OpenAL context: " + GetError());
  }
}

void OpenAL::Deinit() noexcept {
  al_device_ = alcGetContextsDevice(al_context_);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(al_context_);
  alcCloseDevice(al_device_);
}

std::string OpenAL::GetError() noexcept {
  ALCenum err = alGetError();
  std::string str;

  switch (err) {
    case ALC_NO_ERROR:
      str = "ALC_NO_ERROR";
      break;
    case ALC_INVALID_DEVICE:
      str = "ALC_INVALID_DEVICE";
      break;
    case ALC_INVALID_CONTEXT:
      str = "ALC_INVALID_CONTEXT";
      break;
    case ALC_INVALID_ENUM:
      str = "ALC_INVALID_ENUM";
      break;
    case ALC_INVALID_VALUE:
      str = "ALC_INVALID_VALUE";
      break;
    case ALC_OUT_OF_MEMORY:
      str = "ALC_OUT_OF_MEMORY";
      break;
    default:
      str = "Unknown error";
      break;
  }
  return str;
}

auto OpenAL::Muted() -> bool& {
  return muted_;
}

