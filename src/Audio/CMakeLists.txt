set(AUDIO_SOURCE_FILES 
  Audio.h
  Audio.cpp
  Effect.h
  Effect.cpp
  Song.h
  Song.cpp
)

add_library(Audio ${AUDIO_SOURCE_FILES})
source_group("Source Files" FILES ${AUDIO_SOURCE_FILES})

# TODO(seidl):
# change headers to correct paths
target_include_directories(Audio 
  PRIVATE ../
  PUBLIC ../FFMPEG
)

target_link_libraries(Audio 
  PRIVATE ProjectOptions
  PRIVATE System
  PUBLIC FFmpegUtil
  PUBLIC OpenAL:OpenAL
  PRIVATE SWRESAMPLE:SWRESAMPLE
  PRIVATE SDL2:SDL2
)
