add_library(FileSystem
  FileSystem.cpp
  )
target_compile_features(FileSystem PRIVATE cxx_std_11)
target_link_libraries(FileSystem 
  PRIVATE Config
  PUBLIC ghc_filesystem
  PRIVATE project_warnings
  PRIVATE project_options)
target_include_directories(FileSystem PUBLIC .)



