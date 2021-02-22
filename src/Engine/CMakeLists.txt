add_library(Engine
  Engine.cpp
  Audio/AudioEngine.cpp
  Video/VideoEngine.cpp
  Assets/AssetLoader.cpp
  ../System/Logger.cpp
  ../Parsers/CfgFile.cpp
  ../Parsers/Parser.cpp
  #../Parsers/AniFile.cpp
  #  ../Parsers/Gm1File.cpp
  #  ../Parsers/TgxFile.cpp
  ../Parsers/TexFile.cpp
  ../Parsers/MlbFile.cpp
  ../System/FileUtil.cpp
)

target_compile_features(Engine PRIVATE cxx_std_11)
target_include_directories(Engine 
  PRIVATE "${${PROJECT_NAME}_SOURCE_DIR}/src/")

target_link_libraries(Engine
  PUBLIC SDL2::SDL2
  PUBLIC OpenAL::OpenAL
  PUBLIC ghc_filesystem
  #PRIVATE project_warnings
  PRIVATE project_options)
 

