set(RENDERING_SOURCE_FILES
  BinkVideo.h
  BinkVideo.cpp
  Camera.h
  Camera.cpp
  Display.h
  Display.cpp
  Font.h
  Font.cpp
  Renderer.h
  Renderer.cpp
  Surface.h
  Surface.cpp
  Texture.h
  Texture.cpp
  TextureAtlas.h
  TextureAtlas.cpp
  Tileset.h
  Tileset.cpp
  Color.h
  Color.cpp
)

add_library(Rendering ${RENDERING_SOURCE_FILES})
source_group("Source Files" FILES ${RENDERING_SOURCE_FILES})

target_include_directories(Rendering 
  PRIVATE ../
  PRIVATE ../FFMPEG
)

target_link_libraries(Rendering 
  PRIVATE ProjectOptions
  PRIVATE System
  PRIVATE FFmpegUtil
  PUBLIC Common
  PUBLIC Sourcehold::SDL 
  PRIVATE OpenAL::OpenAL
)

add_library(Sourcehold::Rendering ALIAS Rendering)
