#pragma once

extern "C" {
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <stdexcept>
#include <memory>

namespace Details {
struct FFmpegDeleters {
  void operator()(AVFormatContext* context) {
    avformat_close_input(&context);
  }

  void operator()(AVCodecContext* context) {
    avcodec_free_context(&context);
  }

  void operator()(AVFrame* frame) {
    av_frame_free(&frame);
  }
  void operator()(SwsContext* context) {
    sws_freeContext(context);
  }
};
}  // namespace Details
namespace Sourcehold {
namespace FFMPEG {
using AVFormatContextUQ =
    std::unique_ptr<AVFormatContext, Details::FFmpegDeleters>;
using AVCodecContextUQ =
    std::unique_ptr<AVCodecContext, Details::FFmpegDeleters>;
using AVFrameUQ = std::unique_ptr<AVFrame, Details::FFmpegDeleters>;
using SWSContextUQ = std::unique_ptr<SwsContext, Details::FFmpegDeleters>;
/*
 * Init the avcodec context needed by the BinkVideo class
 */
bool InitAvcodec();

/*
 * Destroy a previously created avcodec context and free
 * associated resources
 */
void DestroyAvcodec();

AVInputFormat* GetAVInputFormat();
}  // namespace FFMPEG
}  // namespace Sourcehold
