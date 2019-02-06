cmake_minimum_required(VERSION 3.2)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Project
project(Stronghold)
set(Stronghold_VERSION, 0.0.1)

include_directories(src)

# Find SDL2
find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 has to be installed!")
endif(NOT SDL2_FOUND)
include_directories(${SDL2_INCLUDE_DIR})

# Find BOOST
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(Boost 1.45.0 COMPONENTS system thread)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else(Boost_FOUND)
	message(FATAL_ERROR "Boost has to be installed!")
endif(Boost_FOUND)

# Find SDL mixer
find_package(SDL2_mixer REQUIRED)
if(SDLMIXER_FOUND)
	include_directories(${SDL2_Mixer_INCLUDE_DIRS})
else(SDLMIXER_FOUND)
	message(FATAL_ERROR "SDL_mixer has to be installed!")
endif(SDLMIXER_FOUND)

# Find FFMPEG
find_package(FFmpeg REQUIRED)
if(FFMPEG_FOUND)
	include_directories(${FFMPEG_INCLUDE_DIRS})
if(NOT SWSCALE_FOUND)
	message(FATAL_ERROR "libswscale has to be installed!")
else()
	include_directories(${SWSCALE_INCLUDE_DIRS})
endif()
else(FFMPEG_FOUND)
	message(FATAL_ERROR "FFMPEG has to be installed!")
endif(FFMPEG_FOUND)

# Include sources
file(GLOB_RECURSE SOURCES src/*.cpp)

# Fixes legacy stuff (i.e. strcmp)
if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Link our exe
add_executable(Stronghold ${SOURCES})
add_definitions(-Wall -Wextra -pedantic-errors -Ofast)
set_target_properties(Stronghold PROPERTIES
 	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

target_link_libraries(
	Stronghold 
	${SDL2_LIBRARY}
	${Boost_LIBRARIES}
	${SDLMIXER_LIBRARY}
	${FFMPEG_LIBRARIES}
	${SWSCALE_LIBRARIES}
)
