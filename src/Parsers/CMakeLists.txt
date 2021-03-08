add_library(Parsers 
  AniFile.cpp
  CfgFile.cpp
  Gm1File.cpp
  HlpFile.cpp
  MapFile.cpp
  MlbFile.cpp
  Parser.cpp
  TexFile.cpp
  TgxFile.cpp
  VolumeTxt.cpp
)

target_compile_features(Parsers PRIVATE cxx_std_11)
target_include_directories(Parsers PRIVATE ../)
target_link_libraries(Parsers 
  PRIVATE ProjectOptions
  PRIVATE blast
  PRIVATE System
  PRIVATE Rendering)
