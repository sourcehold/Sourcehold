function(AddImportedTarget NAME LIBRARY INCLUDE)
# Mac OS frameworks should be handled in a special way.
# See more info about cmake issues with Mac OS frameworks here:
# https://cmake.org/pipermail/cmake/2016-April/063179.html
# and here
# https://gitlab.kitware.com/cmake/cmake/-/issues/19410
  if(TARGET_OS_MAC_OS AND ${LIBRARY} MATCHES "\\.framework$")
    add_library(${NAME} INTERFACE IMPORTED)
    set_target_properties(${NAME} PROPERTIES
      INTERFACE_LINK_LIBRARIES ${LIBRARY}
    )
  else()
    add_library(${NAME} UNKNOWN IMPORTED)
    set_target_properties(${NAME} PROPERTIES
      IMPORTED_LOCATION ${LIBRARY}
    )
  endif()
  set_target_properties(${NAME} PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${INCLUDE}
  )
endfunction()
