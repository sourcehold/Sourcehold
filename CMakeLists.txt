cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

#if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND APPLE)
    set(MAC_OS TRUE)
#endif()

# Project
project(Stronghold)
set(Stronghold_VERSION, 0.1.2)

if(${PROJECT_SOURCE_DIR} MATCHES ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "Please run cmake from a separate directory!")
endif()

include_directories(src)

# Thirdparty
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/blast/)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/duktape/)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/cxxopts/include/)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/filesystem/include/)

# pthread
find_package (Threads)

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

# OpenAL
find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})

# FFmpeg
find_package(FFmpeg REQUIRED)
include_directories(${FFMPEG_INCLUDE_DIRS})
include_directories(${SWSCALE_INCLUDE_DIRS})

# swresample
find_package(Libswresample REQUIRED)
include_directories( ${LIBSWRESAMPLE_INCLUDE_DIRS})

# Include sources / headers
file(
    GLOB_RECURSE _source_list 
    LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/src/*.cpp*"
    "${CMAKE_SOURCE_DIR}/src/*.h*"
    "${CMAKE_SOURCE_DIR}/thirdparty/blast/*.c"
    "${CMAKE_SOURCE_DIR}/thirdparty/blast/*.h"
    "${CMAKE_SOURCE_DIR}/thirdparty/cxxopts/*.c"
    "${CMAKE_SOURCE_DIR}/thirdparty/cxxopts/*.h"
)

foreach(_source IN ITEMS ${_source_list})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${CMAKE_SOURCE_DIR}/src/" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
		string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
		set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
		set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
		set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
	endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
else()
	add_definitions(-Wno-reorder -pedantic-errors -Ofast -fno-fast-math)
endif()

if(MAC_OS)
    list(APPEND _source_list "${CMAKE_SOURCE_DIR}/macos/src/PathUtils.m")
endif()

add_executable(Stronghold ${_source_list})
set_target_properties(Stronghold PROPERTIES
 	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

target_link_libraries(
	Stronghold
	${SDL2_LIBRARY}
	${OPENAL_LIBRARY}
	${FFMPEG_LIBRARIES}
	${SWSCALE_LIBRARIES}
	${LIBSWRESAMPLE_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)

if(MAC_OS)
    target_link_libraries(Stronghold 
        "-framework Foundation"
    )
endif()
