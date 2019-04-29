cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Project
project(Stronghold)
set(Stronghold_VERSION, 0.1.0)

if(${PROJECT_SOURCE_DIR} MATCHES ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "Please run cmake from a separate directory!")
endif()

include_directories(src)

# Thirdparty
include_directories(thirdparty/cxxopts/include/)

# Find pthread
find_package (Threads)

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

# Find boost
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME ON) 
find_package(Boost 1.45.0 COMPONENTS filesystem) 
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS}) 
else(Boost_FOUND)
	message(FATAL_ERROR "the following boost components need to be installed: fileystem")
endif(Boost_FOUND)

# Include sources
file(GLOB_RECURSE SOURCES src/*.cpp)

# Fixes legacy stuff (i.e. strcmp)
if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
	add_definitions(-Wno-reorder -pedantic-errors -Ofast)
endif()

# Link our exe
add_executable(Stronghold ${SOURCES})
set_target_properties(Stronghold PROPERTIES
 	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

target_link_libraries(
	Stronghold
	${SDL2_LIBRARY}
	${Boost_LIBRARIES}
	${OPENAL_LIBRARY}
	${FFMPEG_LIBRARIES}
	${SWSCALE_LIBRARIES}
	${LIBSWRESAMPLE_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)
