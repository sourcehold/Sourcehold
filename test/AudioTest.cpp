#include <gtest/gtest.h>
#include <Audio/Audio.h>
using namespace Sourcehold::Audio;
TEST(Audio, PlayWav) {
  Audio::Init();

  auto source = AudioSource<1>();

  Audio::Deinit();
}
