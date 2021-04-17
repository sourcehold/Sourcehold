#include <stdexcept>

#include "AudioSource.h"

using namespace Sourcehold::OpenAL;

template <size_t NumAudioBuffers>
AudioSource<NumAudioBuffers>::AudioSource(ALuint num_channels, ALenum format,
                                          ALsizei sample_rate)
    : al_num_channels_(num_channels),
      al_format_(format),
      al_sample_rate(sample_rate) {
  alGenSources(1, &al_source_);

  alGenBuffers(NumAudioBuffers, al_buffers_.data());

  alSource3f(al_source_, AL_POSITION, .0f, .0f, .0f);
  alSource3f(al_source_, AL_VELOCITY, .0f, .0f, .0f);
  alSourcef(al_source_, AL_PITCH, 1.f);
  alSourcef(al_source_, AL_GAIN, 1.f);

  al_buffers_free_.reserve(NumAudioBuffers);
  for (auto buf : al_buffers_) {
    al_buffers_free_.emplace_back(buf);
  }
}

template <size_t NumAudioBuffers>
AudioSource<NumAudioBuffers>::~AudioSource() {
  alDeleteBuffers(NumAudioBuffers, al_buffers_.data());
  alSourceStop(al_source_);
  alDeleteSources(1, &al_source_);
}

template <size_t NumAudioBuffers>
auto AudioSource<NumAudioBuffers>::Update(const std::vector<float>& audio_data)
    -> void {
  UnqueueAudioBuffers();
  if (al_buffers_free_.empty()) {
    throw std::runtime_error("no free audio buffers");
  }
  alBufferData(al_buffers_free_.back(), al_format_, audio_data.data(),
               static_cast<ALsizei>(audio_data.size()), al_sample_rate);
  alSourceQueueBuffers(al_source_, 1, &al_buffers_free_.back());
  al_buffers_free_.pop_back();
}

template <size_t NumAudioBuffers>
auto AudioSource<NumAudioBuffers>::UnqueueAudioBuffers() -> void {
  ALint available_buffers = 0;
  alGetSourcei(al_source_, AL_BUFFERS_PROCESSED, &available_buffers);

  std::vector<ALuint> freed_buffers(static_cast<size_t>(available_buffers));

  alSourceUnqueueBuffers(al_source_, available_buffers, freed_buffers.data());
  for (auto free_buf : freed_buffers) {
    al_buffers_free_.emplace_back(free_buf);
  }
}
template class Sourcehold::OpenAL::AudioSource<1>;
template class Sourcehold::OpenAL::AudioSource<2>;
template class Sourcehold::OpenAL::AudioSource<3>;
template class Sourcehold::OpenAL::AudioSource<4>;
template class Sourcehold::OpenAL::AudioSource<5>;
template class Sourcehold::OpenAL::AudioSource<6>;
template class Sourcehold::OpenAL::AudioSource<7>;
template class Sourcehold::OpenAL::AudioSource<8>;
template class Sourcehold::OpenAL::AudioSource<9>;
template class Sourcehold::OpenAL::AudioSource<10>;
template class Sourcehold::OpenAL::AudioSource<11>;
template class Sourcehold::OpenAL::AudioSource<12>;
template class Sourcehold::OpenAL::AudioSource<13>;
template class Sourcehold::OpenAL::AudioSource<14>;
template class Sourcehold::OpenAL::AudioSource<15>;
template class Sourcehold::OpenAL::AudioSource<16>;
