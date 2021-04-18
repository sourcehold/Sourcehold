#include <gtest/gtest.h>
#include <OpenAL/AudioSource.h>
#include <FFMPEG/Decoder.h>

using namespace Sourcehold::OpenAL;
using namespace Sourcehold::FFMPEG;

TEST(Audio, PlayWav) {
  OpenAL::Init();

  auto decoder = DecoderAudio("/Users/fseidl/Projects/Sourcehold/data/fx/mill.wav", av_find_input_format("wav"));
  decoder.AudioInfo();
  auto source = AudioSource<1,short>(decoder.audio_context_->channels, AL_FORMAT_STEREO16, decoder.audio_context_->sample_rate);
  
  AVPacket packet;
  
  std::vector<short> result;
  
  while(1){
    av_init_packet(&packet);
      
    if (av_read_frame(decoder.av_context_.get(), &packet) < 0) {
      break;
    }
    
    if(avcodec_send_packet(decoder.audio_context_.get(), &packet)){
      av_frame_unref(decoder.audio_frame_.get());

      auto state = avcodec_receive_frame(decoder.audio_context_.get(), decoder.audio_frame_.get());

      while(state >= 0){
        if (state == AVERROR(EAGAIN) || state == AVERROR_EOF) {
          break;
          }
        else if (state < 0) {
          throw std::runtime_error("failed to recieve frame from audio decoder: " +
                                 decoder.GetAvError(state));
        }
        for(auto c = 0; c < 2; ++c){
          for(auto i = 0; i < decoder.audio_frame_->nb_samples; ++i){
            result.emplace_back(decoder.audio_frame_->extended_data[c][i]);
          }
        }
        
        state = avcodec_receive_frame(decoder.audio_context_.get(), decoder.audio_frame_.get());
      }
    }
  }
  source.Update(result);
  
  source.Play();
  while(source.IsPlaying());
  
  OpenAL::Deinit();
}
