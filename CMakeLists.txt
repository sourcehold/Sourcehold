cmake_minimum_required(VERSION 3.0)

# Project
project(Stronghold)


include(cmake/Preamble.cmake)

add_subdirectory(apple)
add_subdirectory(src)

if(SOURCEHOLD_BUILD_TESTS MATCHES ON)
  add_subdirectory(test)
endif()

# TODO(seidl):
# still some modules hidden here..
set(SOURCE_FILES 
  src/Assets.h
  src/Assets.cpp
  src/Building.h
  src/Building.cpp
  src/Game.cpp
  src/GameManager.h
  src/GameManager.cpp
  src/GameMap.h
  src/GameMap.cpp
  src/Startup.h
  src/Startup.cpp
  src/World.h
  src/World.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
source_group("Source Files" FILES ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
  PRIVATE src/
)

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
