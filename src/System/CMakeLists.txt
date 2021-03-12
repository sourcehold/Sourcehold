add_library(System
  FileUtil.cpp
  Logger.cpp
  ModCampaign.cpp
)
target_include_directories(System PRIVATE ../)
target_link_libraries(System 
  PRIVATE ProjectOptions
  PUBLIC ghc_filesystem
)

if(TARGET_OS_MAC_OS OR TARGET_OS_IOS)
target_link_libraries(System
  PRIVATE AppleUtils
)
endif() # TARGET_OS_MAC_OS OR TARGET_OS_IOS