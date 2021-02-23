add_subdirectory(AssetsDef)
add_subdirectory(Parsers)

add_library(AssetLoader AssetLoader.cpp)
target_compile_features(AssetLoader PRIVATE cxx_std_11)
target_link_libraries(AssetLoader 
  PRIVATE Logger
  PRIVATE FileSystem
  PRIVATE GameOptions
  PRIVATE Parsers
  PUBLIC AssetsDef
  PRIVATE project_warnings
  PRIVATE project_options
  )
