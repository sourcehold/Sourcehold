set(RENDERING_SOURCE_FILES
  BinkVideo.cpp
  Camera.cpp
  Display.cpp
  Font.cpp
  Renderer.cpp
  Surface.cpp
  Texture.cpp
  TextureAtlas.cpp
  Tileset.cpp
)

add_library(Rendering ${RENDERING_SOURCE_FILES})
source_group("Source Files" FILES ${RENDERING_SOURCE_FILES})

target_include_directories(Rendering 
  PRIVATE ../
  PRIVATE ../FFMPEG
)

target_link_libraries(Rendering 
  PRIVATE ProjectOptions
  PUBLIC System
  PUBLIC FFmpegUtil 
  PUBLIC SDLBackend
)
