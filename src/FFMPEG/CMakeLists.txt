add_library(FFmpegUtil
  FFmpegUtil.cpp)
target_include_directories(FFmpegUtil PRIVATE ../)
target_link_libraries(FFmpegUtil
  PRIVATE ProjectOptions
  PRIVATE System
  PUBLIC FFMPEG::AVCODEC
  PUBLIC FFMPEG::AVFORMAT
  PUBLIC FFMPEG::AVUTIL
  PUBLIC FFMPEG::SWSCALE 
)

