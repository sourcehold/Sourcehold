#pragma once

#include "FFMPEG/BinkStream.h"

#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"

namespace Sourcehold {
namespace Rendering {
/*
 * A single bink video file, exposes a texture as a frame buffer
 */
class BinkVideo : public TextureVideo {
 public:
  BinkVideo() = delete;
  BinkVideo(std::unique_ptr<FFMPEG::BinkStream> bink_stream);
  ~BinkVideo() = default;

  void Update();

  bool IsRunning() {
    return bink_stream_->running_;
  }

 protected:
  void test();

 private:
  std::unique_ptr<FFMPEG::BinkStream> bink_stream_;
};
}  // namespace Rendering
}  // namespace Sourcehold
