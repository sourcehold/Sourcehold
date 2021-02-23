add_library(Parsers 
  #  AniFile.cpp
  CfgFile.cpp
  #Gm1File.cpp
  #HlpFile.cpp
  #MapFile.cpp
  MlbFile.cpp
  TexFile.cpp
  Parser.cpp
  #TgxFile.cpp
  )
target_compile_features(Parsers PRIVATE cxx_std_11)

target_link_libraries(Parsers
  PRIVATE FileSystem
  PUBLIC AssetsDef
  PRIVATE Logger
  PRIVATE project_warnings
  PRIVATE project_options)
