add_library(AudioEngine AudioEngine.cpp)
target_compile_features(AudioEngine PRIVATE cxx_std_11)

target_link_libraries(AudioEngine 
  PRIVATE GameOptions
  PRIVATE Logger
  PUBLIC OpenAL::OpenAL
  PRIVATE project_warnings
  PRIVATE project_options)
