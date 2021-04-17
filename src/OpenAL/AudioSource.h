#pragma once

#include "OpenAL.h"
#include <vector>
#include <array>

namespace Sourcehold {
namespace OpenAL {
template <size_t NumAudioBuffers>
class AudioSource {
 public:
  AudioSource(ALuint num_channels, ALenum format, ALsizei sample_rate);
  ~AudioSource();

  void Update(const std::vector<float>& audio_data);

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
