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
  src/Game.cpp
  src/GameManager.cpp
  src/GameMap.cpp
  src/Startup.cpp
  src/World.cpp
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
  PRIVATE ProjectOptions
)
