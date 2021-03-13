set(SYSTEM_SOURCE_FILES
  Config.h
  filesystem.h
  FileUtil.h
  FileUtil.cpp
  Logger.h
  Logger.cpp
  ModCampaign.h
  ModCampaign.cpp
  System.h
)

add_library(System ${SYSTEM_SOURCE_FILES})
source_group("Source Files" FILES ${SYSTEM_SOURCE_FILES})

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
