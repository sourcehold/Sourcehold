set(PARSERS_SOURCE_FILES
  AniFile.h
  AniFile.cpp
  CfgFile.h
  CfgFile.cpp
  Gm1File.h
  Gm1File.cpp
  HlpFile.h
  HlpFile.cpp
  MapFile.h
  MapFile.cpp
  MlbFile.h
  MlbFile.cpp
  Parser.h
  Parser.cpp
  Riff.h
  TexFile.h
  TexFile.cpp
  TgxFile.h
  TgxFile.cpp
  VolumeTxt.h
  VolumeTxt.cpp
)

add_library(Parsers ${PARSERS_SOURCE_FILES})
source_group("Source Files" FILES ${PARSERS_SOURCE_FILES})

target_include_directories(Parsers PRIVATE ../)
target_link_libraries(Parsers 
  PRIVATE ProjectOptions
  PRIVATE blast
  PRIVATE System
  PRIVATE Rendering
)
