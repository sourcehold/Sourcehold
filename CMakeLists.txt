cmake_minimum_required(VERSION 3.0)

# Project
project(Stronghold)
set(Stronghold_VERSION, 0.1.2)


include(cmake/Preamble.cmake)

add_subdirectory(macos)
add_subdirectory(src)

# TODO(seidl):
# still some modules hidden here..
add_executable(Stronghold 
  src/Assets.cpp
  src/Building.cpp
  src/FFmpegUtil.cpp
  src/Game.cpp
  src/GameManager.cpp
  src/GameMap.cpp
  src/Startup.cpp
  src/World.cpp
)

if(MSVC)
  target_compile_options(Stronghold PRIVATE -D_CRT_SECURE_NO_WARNINGS)
  foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
  endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
else()
  target_compile_options(Stronghold PRIVATE
    -Wno-reorder
    -pedantic-errors
    -Ofast
    -fno-fast-math)
endif()


set_target_properties(Stronghold PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

if(MAC_OS)
  target_link_libraries(Stronghold
    PRIVATE MacOSUtils
  )
endif()

target_include_directories(Stronghold PRIVATE src/)
target_link_libraries(Stronghold
  PRIVATE Threads::Threads
  PRIVATE Rendering
  PRIVATE cxxopts
  PRIVATE ECS
  PRIVATE System
  PRIVATE Audio
  PRIVATE GUI
  PRIVATE Events
  PRIVATE Parsers
)
