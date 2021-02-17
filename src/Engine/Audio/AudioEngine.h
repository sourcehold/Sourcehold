#ifndef AUDIO_ENGINE_H_
#define AUDIO_ENGINE_H_
#include <al.h>
#include <alc.h>
class AudioEngine {
 public:
  AudioEngine();
  ~AudioEngine();

  bool muted_;

 private:
  void InitOpenAL();
  ALCdevice* device_;
  ALCcontext* context_;
};
#endif  // AUDIO_ENGINE_H_
