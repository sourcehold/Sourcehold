if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin" AND APPLE)
  message("Building for Mac OS...")

  set(MAC_OS TRUE)


# MacOSUtils
add_library(MacOSUtils src/PathUtils.m)
target_link_libraries(MacOSUtils
  PRIVATE "-framework Foundation"
)

endif() # If Darwin or APPLE


