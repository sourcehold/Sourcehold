# PREAMBLE
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(SOURCEHOLD_BUILD_TESTS "Build Sourcehold tests" ON)

if(MSVC)
  foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
  endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
endif()


include(cmake/PreventInSourceBuild.cmake)
include(cmake/PlatformCheck.cmake)
#include(cmake/AddImportedTarget.cmake)
include(cmake/Dependencies.cmake)
include(cmake/ProjectOptions.cmake)
