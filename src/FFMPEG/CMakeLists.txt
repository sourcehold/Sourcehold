set(FFMPEG_UTIL_SOURCE_FILES
  FFmpegUtil.h
  FFmpegUtil.cpp
)

add_library(FFmpegUtil ${FFMPEG_UTIL_SOURCE_FILES})
source_group("Source Files" FILES ${FFMPEG_UTIL_SOURCE_FILES})

target_include_directories(FFmpegUtil PRIVATE ../)

target_link_libraries(FFmpegUtil
  PRIVATE ProjectOptions
  PRIVATE System
  PUBLIC FFMPEG:AVCODEC
  PUBLIC FFMPEG:AVFORMAT
  PUBLIC FFMPEG:AVUTIL
  PUBLIC FFMPEG:SWSCALE 
)

