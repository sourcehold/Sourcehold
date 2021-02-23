add_library(Config INTERFACE)
target_include_directories(Config INTERFACE "${CMAKE_CURRENT_LIST_DIR}/Config/")

add_library(GameOptions INTERFACE)
target_include_directories(GameOptions INTERFACE "${CMAKE_CURRENT_LIST_DIR}/GameOptions")


add_subdirectory(Logger)
add_subdirectory(FileSystem)
add_subdirectory(Video)
add_subdirectory(Audio)
add_subdirectory(Assets)

add_library(Engine Engine.cpp)
target_compile_features(Engine PRIVATE cxx_std_11)
target_link_libraries(Engine
  PUBLIC Logger
  PUBLIC VideoEngine
  PUBLIC AudioEngine
  PUBLIC AssetLoader
  PUBLIC GameOptions
  PUBLIC FileSystem
  PUBLIC Config
  PRIVATE project_warnings
  PRIVATE project_options)
