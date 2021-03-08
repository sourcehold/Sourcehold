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
target_include_directories(Rendering 
  PRIVATE ../
  PRIVATE ../FFMPEG)

target_link_libraries(Rendering 
  PRIVATE ProjectOptions
  PRIVATE System
  PRIVATE FFmpegUtil
  PUBLIC SDL2::SDL2
  PRIVATE OpenAL::OpenAL)
