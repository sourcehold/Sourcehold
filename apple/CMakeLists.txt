if(TARGET_OS_MAC_OS OR TARGET_OS_IOS)
# AppleUtils
  set(APPLE_UTILS_SOURCE_FILES 
    Common/SHPathUtils.h
    Common/SHPathUtils.m
  )

  add_library(AppleUtils ${APPLE_UTILS_SOURCE_FILES})
  source_group("Source Files" FILES ${APPLE_UTILS_SOURCE_FILES})

  target_include_directories(AppleUtils
    PUBLIC ${CMAKE_SOURCE_DIR}/apple
  )

  target_link_libraries(AppleUtils
    PRIVATE "-framework Foundation"
    PRIVATE ProjectOptions
  )
endif() # TARGET_OS_MAC_OS OR TARGET_OS_IOS
