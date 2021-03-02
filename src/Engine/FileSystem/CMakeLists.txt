add_library(FileSystem
  FileSystem.cpp
  FileStream.cpp
  )
target_compile_features(FileSystem PRIVATE cxx_std_11)
target_link_libraries(FileSystem 
  PUBLIC ghc_filesystem
  PRIVATE Config
  PRIVATE project_warnings
  PRIVATE project_options)
target_include_directories(FileSystem PUBLIC .)



