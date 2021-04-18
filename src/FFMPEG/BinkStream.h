#pragma once
#include <functional>

#include "FFMPEG/Decoder.h"
#include "Common/Shapes.h"

#include "OpenAL/AudioSource.h"

namespace Sourcehold {
namespace FFMPEG {
class BinkStream {
 public:
  BinkStream(const std::string& path, bool looping = false);
  ~BinkStream() = default;

  void Update();
  bool Finished();

  std::vector<uint32_t> frame_buffer_;
  Vector2<int> buffer_dim;
  bool running_;

  std::function<void()> OnUpdateVideo_;

 private:
  void UpdateVideo();
  void UpdateAudio();

  bool VideoFinished();
  void ProcessVideoFrame();

  bool AudioState();
  std::vector<float> GetAudioData();

  std::unique_ptr<DecoderAll> decoder_;

  AVPacket packet_;
  bool packet_finished_;

  std::unique_ptr<OpenAL::AudioSource<17,float>> audio_source_;
  SWSContextUQ sws_context_;

  bool looping_;
};
}  // namespace FFMPEG
}  // namespace Sourcehold

