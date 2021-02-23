add_library(VideoEngine
  VideoEngine.cpp)

target_compile_features(VideoEngine PRIVATE cxx_std_11)
target_link_libraries(VideoEngine 
  PUBLIC SDL2::SDL2
  PRIVATE GameOptions
  PRIVATE Logger
  PRIVATE Config
  PRIVATE project_warnings
  PRIVATE project_options)
