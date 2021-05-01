set(SOURCEHOLD_SDL_SOURCE_FILES
  SDLBackend.h
  SDLBackend.cpp
  SDLHelpers.h
  SDLFactories.h
  SDLFactories.cpp
)

add_library(SourceholdSDL ${SOURCEHOLD_SDL_SOURCE_FILES})
source_group("Source Files" FILES ${SOURCEHOLD_SDL_SOURCE_FILES})

target_include_directories(SourceholdSDL
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(SourceholdSDL
  PRIVATE ProjectOptions
  PRIVATE System
  PUBLIC SDL2::SDL2
  PUBLIC Common
)

add_library(Sourcehold::SDL ALIAS SourceholdSDL)
