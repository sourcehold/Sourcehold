#include "BinkVideo.h"
#include <iostream>
using namespace Sourcehold::Rendering;
using namespace Sourcehold::FFMPEG;

BinkVideo::BinkVideo(std::unique_ptr<BinkStream> bink_stream)
    : TextureVideo(bink_stream->buffer_dim),
      bink_stream_(std::move(bink_stream)) {
  bink_stream_->OnUpdateVideo_ = [&]() {
    auto accessor = Accessor();

    std::copy(std::begin(bink_stream_->frame_buffer_),
              std::end(bink_stream_->frame_buffer_), std::begin(accessor));
  };
}

void BinkVideo::Update() {
  bink_stream_->Update();
}
