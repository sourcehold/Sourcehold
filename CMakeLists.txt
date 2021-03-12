cmake_minimum_required(VERSION 3.0)

# Project
project(Stronghold VERSION 0.1.2)


include(cmake/Preamble.cmake)

add_subdirectory(apple)
add_subdirectory(src)

# TODO(seidl):
# still some modules hidden here..
add_executable(${PROJECT_NAME}
  src/Assets.cpp
  src/Building.cpp
  src/Game.cpp
  src/GameManager.cpp
  src/GameMap.cpp
  src/Startup.cpp
  src/World.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src/)

if(TARGET_OS_MAC_OS OR TARGET_OS_IOS)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE AppleUtils
  )
endif() #TARGET_OS_MAC_OS OR TARGET_OS_IOS

if(TARGET_OS_IOS)
  include(apple/cmake/MakeiOSAppBundleAndLinkFrameworks.cmake)
endif() # TARGET_OS_IOS

target_link_libraries(${PROJECT_NAME}
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
