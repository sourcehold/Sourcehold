if(MAC_OS)
  message("Building for Mac OS...")
# MacOSUtils
  add_library(MacOSUtils src/PathUtils.m)
  target_link_libraries(MacOSUtils
    PRIVATE "-framework Foundation"
    PRIVATE ProjectOptions
)
endif() # MAC_OS
