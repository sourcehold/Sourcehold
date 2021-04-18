#include <stdexcept>
#include "AudioSource.h"

using namespace Sourcehold::OpenAL;

template <size_t N, typename T>
AudioSource<N, T>::AudioSource(ALuint num_channels, ALenum format,
                                          ALsizei sample_rate)
    : al_num_channels_(num_channels),
      al_format_(format),
      al_sample_rate(sample_rate) {
  alGenSources(1, &al_source_);

  alGenBuffers(N, al_buffers_.data());

  Position({.0f, .0f, .0f});
  Velocity({.0f, .0f, .0f});
  Pitch(1.f);
  Gain(1.f);

  al_buffers_free_.reserve(N);
  for (auto buf : al_buffers_) {
    al_buffers_free_.emplace_back(buf);
  }
}

template <size_t N, typename T>
AudioSource<N,T>::~AudioSource() {
  alDeleteBuffers(N, al_buffers_.data());
  alSourceStop(al_source_);
  alDeleteSources(1, &al_source_);
}

template <size_t N, typename T>
auto AudioSource<N,T>::Play() -> void {
  alSourcePlay(al_source_);
}

template <size_t N, typename T>
auto AudioSource<N,T>::Pause() -> void {
  alSourcePause(al_source_);
}

template <size_t N, typename T>
auto AudioSource<N,T>::Rewind() -> void {
  alSourceRewind(al_source_);
}

template <size_t N, typename T>
auto AudioSource<N,T>::Stop() -> void {
  alSourceStop(al_source_);
}

template <size_t N, typename T>
[[nodiscard]]
auto AudioSource<N,T>::IsPlaying() -> bool {
  ALenum state;
  alGetSourcei(al_source_, AL_SOURCE_STATE, &state);
  return state == AL_PLAYING;
}

template <size_t N, typename T>
auto AudioSource<N,T>::Position(Vector3<float> pos) -> void {
  alSource3f(al_source_, AL_POSITION, pos.x, pos.y, pos.z);
}

template <size_t N, typename T>
[[nodiscard]]
auto AudioSource<N,T>::Position() -> Vector3<float> {
  Vector3<float> pos;
  alGetSource3f(al_source_, AL_POSITION, &pos.x, &pos.y, &pos.z);
  return pos;
}

template <size_t N, typename T>
auto AudioSource<N,T>::Velocity(Vector3<float> vel) -> void {
  alSource3f(al_source_, AL_VELOCITY, vel.x, vel.y, vel.z);
}

template <size_t N, typename T>
[[nodiscard]]
auto AudioSource<N,T>::Velocity() -> Vector3<float> {
  Vector3<float> vel;
  alGetSource3f(al_source_, AL_VELOCITY, &vel.x, &vel.y, &vel.z);
  return vel;
}

template <size_t N, typename T>
auto AudioSource<N,T>::Pitch(float pitch) -> void {
  alSourcef(al_source_, AL_PITCH, pitch);
}

template <size_t N, typename T>
auto AudioSource<N,T>::Pitch() -> float {
  float pitch;
  alGetSourcef(al_source_, AL_PITCH, &pitch);
  return pitch;
}

template <size_t N, typename T>
auto AudioSource<N,T>::Gain(float gain) -> void {
  alSourcef(al_source_, AL_GAIN, gain);
}

template <size_t N, typename T>
auto AudioSource<N,T>::Gain() -> float {
  float gain;
  alGetSourcef(al_source_, AL_GAIN, &gain);
  return gain;
}

template <size_t N, typename T>
auto AudioSource<N,T>::Update(const std::vector<T>& audio_data)
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

template <size_t N, typename T>
auto AudioSource<N, T>::UnqueueAudioBuffers() -> void {
  ALint available_buffers = 0;
  alGetSourcei(al_source_, AL_BUFFERS_PROCESSED, &available_buffers);

  std::vector<ALuint> freed_buffers(static_cast<size_t>(available_buffers));

  alSourceUnqueueBuffers(al_source_, available_buffers, freed_buffers.data());
  for (auto free_buf : freed_buffers) {
    al_buffers_free_.emplace_back(free_buf);
  }
}

template class Sourcehold::OpenAL::AudioSource<1, float>;
template class Sourcehold::OpenAL::AudioSource<2, float>;
template class Sourcehold::OpenAL::AudioSource<3, float>;
template class Sourcehold::OpenAL::AudioSource<4, float>;
template class Sourcehold::OpenAL::AudioSource<5, float>;
template class Sourcehold::OpenAL::AudioSource<6, float>;
template class Sourcehold::OpenAL::AudioSource<7, float>;
template class Sourcehold::OpenAL::AudioSource<8, float>;
template class Sourcehold::OpenAL::AudioSource<9, float>;
template class Sourcehold::OpenAL::AudioSource<10, float>;
template class Sourcehold::OpenAL::AudioSource<11, float>;
template class Sourcehold::OpenAL::AudioSource<12, float>;
template class Sourcehold::OpenAL::AudioSource<13, float>;
template class Sourcehold::OpenAL::AudioSource<14, float>;
template class Sourcehold::OpenAL::AudioSource<15, float>;
template class Sourcehold::OpenAL::AudioSource<16, float>;
template class Sourcehold::OpenAL::AudioSource<17, float>;
template class Sourcehold::OpenAL::AudioSource<18, float>;
template class Sourcehold::OpenAL::AudioSource<19, float>;
template class Sourcehold::OpenAL::AudioSource<20, float>;

template class Sourcehold::OpenAL::AudioSource<1, short>;
template class Sourcehold::OpenAL::AudioSource<2, short>;
template class Sourcehold::OpenAL::AudioSource<3, short>;
template class Sourcehold::OpenAL::AudioSource<4, short>;
template class Sourcehold::OpenAL::AudioSource<5, short>;
template class Sourcehold::OpenAL::AudioSource<6, short>;
template class Sourcehold::OpenAL::AudioSource<7, short>;
template class Sourcehold::OpenAL::AudioSource<8, short>;
template class Sourcehold::OpenAL::AudioSource<9, short>;
template class Sourcehold::OpenAL::AudioSource<10, short>;
template class Sourcehold::OpenAL::AudioSource<11, short>;
template class Sourcehold::OpenAL::AudioSource<12, short>;
template class Sourcehold::OpenAL::AudioSource<13, short>;
template class Sourcehold::OpenAL::AudioSource<14, short>;
template class Sourcehold::OpenAL::AudioSource<15, short>;
template class Sourcehold::OpenAL::AudioSource<16, short>;
template class Sourcehold::OpenAL::AudioSource<17, short>;
template class Sourcehold::OpenAL::AudioSource<18, short>;
template class Sourcehold::OpenAL::AudioSource<19, short>;
template class Sourcehold::OpenAL::AudioSource<20, short>;

template class Sourcehold::OpenAL::AudioSource<1, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<2, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<3, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<4, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<5, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<6, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<7, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<8, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<9, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<10, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<11, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<12, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<13, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<14, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<15, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<16, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<17, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<18, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<19, uint8_t>;
template class Sourcehold::OpenAL::AudioSource<20, uint8_t>;
