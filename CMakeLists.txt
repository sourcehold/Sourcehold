cmake_minimum_required(VERSION 3.2)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Project
project(Stronghold)
set(Stronghold_VERSION, 0.1.0)

include_directories(src)

# Find SDL2
find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 has to be installed!")
endif(NOT SDL2_FOUND)
include_directories(${SDL2_INCLUDE_DIR})

# Find OpenAL
find_package(OpenAL REQUIRED)
if(NOT OPENAL_FOUND)
    message(FATAL_ERROR "OpenAL has to be installed!")
endif(NOT OPENAL_FOUND)
include_directories(${OPENAL_INCLUDE_DIR})

# Find FFMPEG
find_package(FFmpeg REQUIRED)
if(FFMPEG_FOUND)
	include_directories(${FFMPEG_INCLUDE_DIRS})
if(NOT SWSCALE_FOUND)
	message(FATAL_ERROR "libswscale has to be installed (provided by FFmpeg)!")
else()
	include_directories(${SWSCALE_INCLUDE_DIRS})
endif()
else(FFMPEG_FOUND)
	message(FATAL_ERROR "FFMPEG has to be installed!")
endif(FFMPEG_FOUND)

# Find swresample
find_package(Libswresample REQUIRED)
if(NOT LIBSWRESAMPLE_FOUND)
	message(FATAL_ERROR "libswresample has to be installed (provided by FFmpeg)!")
else(NOT LIBSWRESAMPLE_FOUND)
	include_directories( ${LIBSWRESAMPLE_INCLUDE_DIRS})
endif(NOT LIBSWRESAMPLE_FOUND)

# Include sources
file(GLOB_RECURSE SOURCES src/*.cpp)

# Fixes legacy stuff (i.e. strcmp)
if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Link our exe
add_executable(Stronghold ${SOURCES})
add_definitions(-Wall -Wextra -Wno-reorder -pedantic-errors -Ofast)
set_target_properties(Stronghold PROPERTIES
 	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

# Defaults to win32 app
if(WIN32)
	target_compile_options(Stronghold PRIVATE -Wl,-subsystem,console)
endif()

target_link_libraries(
	Stronghold 
	${SDL2_LIBRARY}
	${Boost_LIBRARIES}
	${OPENAL_LIBRARY}
	${FFMPEG_LIBRARIES}
	${SWSCALE_LIBRARIES}
	${LIBSWRESAMPLE_LIBRARIES}
)

# cpack stuff
#SET(CPACK_GENERATOR "DEB")
#SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Julian Offenhäuser")

#SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Open source engine implementation of Stronghold by Firefly Studios")
#SET(CPACK_PACKAGE_VENDOR "Julian Offenhäuser")
#SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
#SET(CPACK_PACKAGE_VERSION_MAJOR "0")
#SET(CPACK_PACKAGE_VERSION_MINOR "1")
#SET(CPACK_PACKAGE_VERSION_PATCH "0")
#SET(CPACK_PACKAGE_INSTALL_DIRECTORY "Sourcehold ${Stronghold_VERSION}")

#INCLUDE(CPack)

