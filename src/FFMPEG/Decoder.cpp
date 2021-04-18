#include "Decoder.h"
#include <iostream>

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

template <DecoderType D>
auto Decoder<D>::AudioInfo() -> void {
  // See the following to know what data type (unsigned char, short, float, etc)
  // to use to access the audio data:
  // http://ffmpeg.org/doxygen/trunk/samplefmt_8h.html#af9a51ca15301871723577c730b5865c5
  std::cout << "Audio frame info:\n"
            << "  Sample count: " << audio_frame_->nb_samples << '\n'
            << "  Channel count: " << audio_context_->channels << '\n'
            << "  Format: "
            << av_get_sample_fmt_name(audio_context_->sample_fmt)
            << '\n'
            << "  Bytes per sample: "
            << av_get_bytes_per_sample(audio_context_->sample_fmt)
            << '\n'
            << "  Is planar? "
            << av_sample_fmt_is_planar(audio_context_->sample_fmt)
            << '\n';

  std::cout
      << "frame->linesize[0] tells you the size (in bytes) of each plane\n";

  if (audio_context_->channels > AV_NUM_DATA_POINTERS &&
      av_sample_fmt_is_planar(audio_context_->sample_fmt)) {
    std::cout
        << "The audio stream (and its frames) have too many channels to fit "
           "in\n"
        << "frame->data. Therefore, to access the audio data, you need to use\n"
        << "frame->extended_data to access the audio data. It's planar, so\n"
        << "each channel is in a different element. That is:\n"
        << "  frame->extended_data[0] has the data for channel 1\n"
        << "  frame->extended_data[1] has the data for channel 2\n"
        << "  etc.\n";
  }
  else {
    std::cout
        << "Either the audio data is not planar, or there is enough room in\n"
        << "frame->data to store all the channels, so you can either use\n"
        << "frame->data or frame->extended_data to access the audio data "
           "(they\n"
        << "should just point to the same data).\n";
  }

  std::cout
      << "If the frame is planar, each channel is in a different element.\n"
      << "That is:\n"
      << "  frame->data[0]/frame->extended_data[0] has the data for channel 1\n"
      << "  frame->data[1]/frame->extended_data[1] has the data for channel 2\n"
      << "  etc.\n";

  std::cout
      << "If the frame is packed (not planar), then all the data is in\n"
      << "frame->data[0]/frame->extended_data[0] (kind of like how some\n"
      << "image formats have RGB pixels packed together, rather than storing\n"
      << " the red, green, and blue channels separately in different arrays.\n";

  std::cout << GetAudioFrameSize() << '\n';
}

template <DecoderType D>
auto Decoder<D>::GetAudioFrameSize() -> int {
  return av_samples_get_buffer_size(nullptr, audio_frame_->channels,
                                    audio_frame_->nb_samples,
                                    audio_context_->sample_fmt, 1);
}

template class Sourcehold::FFMPEG::Decoder<DecoderType::All>;
template class Sourcehold::FFMPEG::Decoder<DecoderType::Audio>;
template class Sourcehold::FFMPEG::Decoder<DecoderType::Video>;
