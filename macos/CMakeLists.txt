if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin" AND APPLE)
  message("Building for Mac OS...")

  set(MAC_OS TRUE)


# MacOSUtils
add_library(MacOSUtils macos/src/PathUtils.m)
set_target_properties(MacOSUtils PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/macos/src)
target_link_libraries(MacOSUtils
  PRIVATE "-framework Foundation"
)

endif() # If Darwin or APPLE


