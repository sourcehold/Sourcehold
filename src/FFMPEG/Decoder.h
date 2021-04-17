#pragma once
#include "FFmpegUtil.h"

namespace Sourcehold {
namespace FFMPEG {

enum class DecoderType { Audio, Video, All };

template <DecoderType D>
class Decoder {
 public:
  Decoder(const std::string& file, AVInputFormat* input_format);
  ~Decoder() = default;

  std::string GetAvError(int state);

  AVFormatContextUQ av_context_;
  AVCodec *video_decoder_, *audio_decoder_;

  AVCodecContextUQ audio_context_;
  AVCodecContextUQ video_context_;

  AVFrameUQ audio_frame_;
  AVFrameUQ video_frame_;

  int av_input;
  int video_stream_;
  int audio_stream_;

  float fps_ = .0f;

 private:
  void CreateFormatContext(const std::string& path,
                           AVInputFormat* input_format);
  void FindStreams();
  void SetFps();
  template <AVMediaType T>
  int SetStream();
  void CreateStreamContext();
  void CreateAudioContext();
  void CreateVideoContext();
};

using DecoderAll = Decoder<DecoderType::All>;
using DecoderAudio = Decoder<DecoderType::Audio>;
using DecoderVideo = Decoder<DecoderType::Video>;
}  // namespace FFMPEG
}  // namespace Sourcehold
