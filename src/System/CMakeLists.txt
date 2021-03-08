add_library(System
  FileUtil.cpp
  Logger.cpp
  ModCampaign.cpp
)
target_include_directories(System PRIVATE ../)
target_compile_features(System PRIVATE cxx_std_11)
target_link_libraries(System 
  PUBLIC ghc_filesystem)
