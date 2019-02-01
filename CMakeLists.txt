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

# Find OpenAL
find_package(OpenAL)
if(OPENAL_FOUND)
	include_directories(${OPENAL_INCLUDE_DIRS})
else(OPENAL_FOUND)
	message(FATAL_ERROR "OpenAL has to be installed!")
endif(OPENAL_FOUND)

# Find ALUT
find_package(ALUT REQUIRED)
if(NOT ALUT_FOUND)
    message(FATAL_ERROR "ALUT has to be installed!")
endif(NOT ALUT_FOUND)
include_directories(${ALUT_INCLUDE_DIR})

# Find FFmpeg (for libavcodec)
find_package(FFmpeg REQUIRED)
if(NOT FFMPEG_FOUND)
    message(FATAL_ERROR "FFmpeg has to be installed!")
endif(NOT FFMPEG_FOUND)
include_directories(${FFMPEG_INCLUDE_DIR})

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
 	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

target_link_libraries(
	Stronghold 
	${SDL2_LIBRARY}
	${Boost_LIBRARIES}
	${OPENAL_LIBRARIES}
	${ALUT_LIBRARIES}
	${FFMPEG_LIBRARIES}
)
