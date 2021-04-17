#include "Decoder.h"

using namespace Sourcehold::FFMPEG;

template <DecoderType D>
Decoder<D>::Decoder(const std::string& path, AVInputFormat* input_format) {
  CreateFormatContext(path, input_format);
  av_context_->max_analyze_duration = 10000;

  FindStreams();
  SetFps();

  CreateStreamContext();
}

template <DecoderType D>
auto Decoder<D>::CreateFormatContext(const std::string& path,
                                     AVInputFormat* input_format) -> void {
  auto av_context = avformat_alloc_context();
  if (!av_context) {
    throw std::runtime_error("Failed to allocate AVFormat context");
  }
  av_input = avformat_open_input(&av_context, path.c_str(),  //
                                 input_format, nullptr);

  if (av_input < 0) {
    throw std::runtime_error("Failed to open AV input: " +
                             GetAvError(av_input));
  }

  av_context_ = AVFormatContextUQ{av_context};
}

template <DecoderType D>
auto Decoder<D>::FindStreams() -> void {
  if (avformat_find_stream_info(av_context_.get(), nullptr) < 0) {
    throw std::runtime_error("Failed to find av stream info");
  }

  switch (D) {
    case DecoderType::All:
      video_stream_ = SetStream<AVMEDIA_TYPE_VIDEO>();
      audio_stream_ = SetStream<AVMEDIA_TYPE_AUDIO>();
      break;
    case DecoderType::Audio:
      audio_stream_ = SetStream<AVMEDIA_TYPE_AUDIO>();
      break;
    case DecoderType::Video:
      video_stream_ = SetStream<AVMEDIA_TYPE_VIDEO>();
      break;
  }
}

template <DecoderType D>
template <AVMediaType A>
auto Decoder<D>::SetStream() -> int {
  static_assert(A == AVMEDIA_TYPE_AUDIO || A == AVMEDIA_TYPE_VIDEO,
                "unsupported stream media type");
  int result = 0;
  switch (A) {
    case AVMEDIA_TYPE_AUDIO:
      result = av_find_best_stream(av_context_.get(),  //
                                   A, -1, -1, &audio_decoder_, 0);
      break;
    case AVMEDIA_TYPE_VIDEO:
      result = av_find_best_stream(av_context_.get(),  //
                                   A, -1, -1, &video_decoder_, 0);
      break;
  }

  if (result < 0) {
    throw std::runtime_error("Failed to find stream: " + GetAvError(result));
  }

  return result;
}

template <DecoderType D>
auto Decoder<D>::SetFps() -> void {
  if constexpr (D == DecoderType::All || D == DecoderType::Video) {
    fps_ = static_cast<float>(
               av_context_->streams[video_stream_]->avg_frame_rate.num) /
           static_cast<float>(
               av_context_->streams[video_stream_]->avg_frame_rate.den);
  }
}

template <DecoderType D>
auto Decoder<D>::CreateStreamContext() -> void {
  switch (D) {
    case DecoderType::All:
      CreateAudioContext();
      CreateVideoContext();
      break;
    case DecoderType::Video:
      CreateVideoContext();
      break;
    case DecoderType::Audio:
      CreateAudioContext();
      break;
  }
}

template <DecoderType D>
auto Decoder<D>::CreateAudioContext() -> void {
  audio_context_ = AVCodecContextUQ(avcodec_alloc_context3(audio_decoder_));
  if (!audio_context_) {
    throw std::runtime_error("Failed to allocate audio context");
  }

  avcodec_parameters_to_context(audio_context_.get(),
                                av_context_->streams[audio_stream_]->codecpar);

  auto err = avcodec_open2(audio_context_.get(), audio_decoder_, nullptr);
  if (err < 0) {
    throw std::runtime_error("Failed to open audio context: " +
                             GetAvError(err));
  }

  audio_frame_ = AVFrameUQ(av_frame_alloc());
  if (!audio_frame_) {
    throw std::runtime_error("Failed to allocate audio frame");
  }
}

template <DecoderType D>
auto Decoder<D>::CreateVideoContext() -> void {
  video_context_ = AVCodecContextUQ(avcodec_alloc_context3(video_decoder_));
  if (!video_context_) {
    throw std::runtime_error("Failed to allocate video context");
  }
  avcodec_parameters_to_context(video_context_.get(),
                                av_context_->streams[video_stream_]->codecpar);

  auto state = avcodec_open2(video_context_.get(), video_decoder_, nullptr);
  if (state < 0) {
    throw std::runtime_error("Failed to open video context: " +
                             GetAvError(state));
  }

  video_frame_ = AVFrameUQ(av_frame_alloc());
  if (!video_frame_) {
    throw std::runtime_error("Failed to allocate video frame");
  }
}

template <DecoderType D>
auto Decoder<D>::GetAvError(int state) -> std::string {
  char msg[100];
  av_make_error_string(msg, 100, state);

  return std::string(msg);
}

template class Sourcehold::FFMPEG::Decoder<DecoderType::All>;
template class Sourcehold::FFMPEG::Decoder<DecoderType::Audio>;
template class Sourcehold::FFMPEG::Decoder<DecoderType::Video>;
