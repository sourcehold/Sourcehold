cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Project
project(Stronghold)
set(Stronghold_VERSION, 0.1.2)

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
endif()
include_directories(${SDL2_INCLUDE_DIR})

# Find OpenAL
find_package(OpenAL REQUIRED)
if(NOT OPENAL_FOUND)
    message(FATAL_ERROR "OpenAL has to be installed!")
endif()
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
else()
	message(FATAL_ERROR "FFMPEG has to be installed!")
endif()

# Find swresample
find_package(Libswresample REQUIRED)
if()
	message(FATAL_ERROR "libswresample has to be installed (provided by FFmpeg)!")
else()
	include_directories( ${LIBSWRESAMPLE_INCLUDE_DIRS})
endif()

# Find boost
set(Boost_USE_STATIC_LIBS ON)
add_definitions(-DBOOST_ALL_NO_LIB)
find_package(Boost COMPONENTS filesystem REQUIRED)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "the following boost components need to be installed: fileystem")
endif()

# Include sources / headers
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
		string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
		set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
		set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
		set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
	endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
else()
	add_definitions(-Wno-reorder -pedantic-errors -Ofast -Wall -Wextra)
endif()

add_executable(Stronghold ${SOURCES} ${HEADERS})
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
