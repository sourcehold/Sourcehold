if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin" AND APPLE)
  if(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
    message("Target platform is iOS.")
    set(TARGET_OS_IOS TRUE)
  else()
    message("Target platform is Mac OS.")
    set(TARGET_OS_MAC_OS TRUE)
  endif()
endif() # If Darwin and APPLE
