set(SDL_BACKEND_SOURCE_FILES
  SDLBackend.cpp
  SDLFactories.cpp
)

add_library(SDLBackend ${SDL_BACKEND_SOURCE_FILES})
source_group("Source Files" FILES ${SDL_BACKEND_SOURCE_FILES})

target_include_directories(SDLBackend
  PRIVATE ../
)

target_link_libraries(SDLBackend
  PRIVATE ProjectOptions
  PRIVATE System
  PUBLIC SDL2::SDL2
)
