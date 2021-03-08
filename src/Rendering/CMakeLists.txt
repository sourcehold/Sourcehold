add_library(Rendering
  BinkVideo.cpp
  Camera.cpp
  Display.cpp
  Font.cpp
  Renderer.cpp
  Surface.cpp
  TextureAtlas.cpp
  Texture.cpp
  Tileset.cpp
)

target_compile_features(Rendering PRIVATE cxx_std_11)
target_include_directories(Rendering PRIVATE ../)

target_link_libraries(Rendering 
  PRIVATE ProjectOptions
  PRIVATE System
  PUBLIC SDL2::SDL2
  PRIVATE FFMPEG::AVCODEC
  PRIVATE FFMPEG::AVFORMAT
  PRIVATE FFMPEG::AVUTIL
  PRIVATE FFMPEG::SWSCALE 
  PRIVATE SWRESAMPLE::SWRESAMPLE
  PRIVATE OpenAL::OpenAL)
