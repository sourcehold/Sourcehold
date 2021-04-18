#pragma once

#include "OpenAL.h"
#include "Vector3.h"

#include <vector>
#include <array>

namespace Sourcehold {
namespace OpenAL {
template <size_t NumAudioBuffers, typename BufferType>
class AudioSource {
 public:
  AudioSource(ALuint num_channels, ALenum format, ALsizei sample_rate);
  ~AudioSource();

  void Play();
  void Pause();
  void Rewind();
  void Stop();
  
  bool IsPlaying();
  bool IsPaused();
  bool IsStopped();
  
  void Position(Vector3<float> pos);
  Vector3<float> Position();
  
  void Velocity(Vector3<float> vel);
  Vector3<float> Velocity();
  
  void Pitch(float pitch);
  float Pitch();
  
  void Gain(float pos);
  float Gain();
  
  void Update(const std::vector<BufferType>& audio_data);
  
 private:
  void UnqueueAudioBuffers();

  ALuint al_source_;
  ALuint al_num_channels_;
  ALenum al_format_;
  ALsizei al_sample_rate;
  
  std::array<ALuint, NumAudioBuffers> al_buffers_;
  std::vector<ALuint> al_buffers_free_;
};
}  // namespace OpenAL
}  // namespace Sourcehold
