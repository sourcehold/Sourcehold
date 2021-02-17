add_library(Engine
  Engine.cpp
  Audio/AudioEngine.cpp
  Video/VideoEngine.cpp
  Assets/AssetLoader.cpp
  ../System/Logger.cpp
  ../Parsers/CfgFile.cpp
  ../Parsers/Parser.cpp
  ../Parsers/AniFile.cpp
  ../Parsers/Gm1File.cpp
  ../Parsers/TgxFile.cpp
)

set_target_properties(Engine PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)
target_link_libraries(Engine
  PUBLIC SDL2::SDL2
  PUBLIC OpenAL::OpenAL
  PUBLIC ghc_filesystem)


