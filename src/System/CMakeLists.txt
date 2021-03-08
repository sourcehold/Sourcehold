add_library(System
  FileUtil.cpp
  Logger.cpp
  ModCampaign.cpp
)
target_include_directories(System PRIVATE ../)
target_link_libraries(System 
  PRIVATE ProjectOptions
  PUBLIC ghc_filesystem)
