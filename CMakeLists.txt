cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin" AND APPLE)
  message("Building for Mac OS...")
  set(MAC_OS TRUE)
endif()

# Project
project(Stronghold)
set(Stronghold_VERSION, 0.1.2)

if(${PROJECT_SOURCE_DIR} MATCHES ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "Please run cmake from a separate directory!")
endif()

include_directories(src)

# Thirdparty

# blast
add_library(blast thirdparty/blast/blast.c)
set_target_properties(blast PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/thirdparty/blast)

# cxxopts
set(CXXOPTS_BUILD_TESTS OFF)
set(CXXOPTS_BUILD_EXAMPLES OFF)

add_subdirectory(thirdparty/cxxopts)

# ghc filesystem
add_subdirectory(thirdparty/filesystem)

# pthread
find_package(Threads REQUIRED)

# SDL2
find_package(SDL2 REQUIRED)

# OpenAL
find_package(OpenAL REQUIRED)

# Mac OS frameworks should be handled in a special way. 
# See more info about cmake issues with Mac OS frameworks here:
# https://cmake.org/pipermail/cmake/2016-April/063179.html
# and here
# https://gitlab.kitware.com/cmake/cmake/-/issues/19410
if(MAC_OS AND OPENAL_LIBRARY MATCHES "\\.framework$")
  add_library(OpenAL::OpenAL INTERFACE IMPORTED)
  set_target_properties(OpenAL::OpenAL PROPERTIES
    INTERFACE_LINK_LIBRARIES "${OPENAL_LIBRARY}")
else()
  add_library(OpenAL::OpenAL UNKNOWN IMPORTED)
  set_target_properties(OpenAL::OpenAL PROPERTIES
    IMPORTED_LOCATION ${OPENAL_LIBRARY})
endif()

set_target_properties(OpenAL::OpenAL PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${OPENAL_INCLUDE_DIR})

# FFmpeg
find_package(FFmpeg REQUIRED COMPONENTS
  AVCODEC AVFORMAT AVUTIL SWSCALE)

# swresample
find_package(Libswresample REQUIRED)
add_library(SWRESAMPLE::SWRESAMPLE UNKNOWN IMPORTED)
set_target_properties(SWRESAMPLE::SWRESAMPLE PROPERTIES
  IMPORTED_LOCATION ${LIBSWRESAMPLE_LIBRARIES}
  INTERFACE_INCLUDE_DIRECTORIES ${LIBSWRESAMPLE_INCLUDE_DIRS})

# MacOSUtils
if(MAC_OS)
  add_library(MacOSUtils apple/Common/SHPathUtils.m)
  set_target_properties(MacOSUtils PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/macos/src)
  target_link_libraries(MacOSUtils
    PRIVATE "-framework Foundation"
  )
endif()

# Include sources / headers
file(
    GLOB_RECURSE _source_list 
    LIST_DIRECTORIES false
    "${CMAKE_SOURCE_DIR}/src/*.cpp*"
    "${CMAKE_SOURCE_DIR}/src/*.h*"
)

foreach(_source IN ITEMS ${_source_list})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${CMAKE_SOURCE_DIR}/src/" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

add_executable(Stronghold ${_source_list})


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

target_link_libraries(Stronghold
  PRIVATE Threads::Threads
  PRIVATE SDL2::SDL2
  PRIVATE OpenAL::OpenAL
  PRIVATE FFMPEG::AVCODEC
  PRIVATE FFMPEG::AVFORMAT
  PRIVATE FFMPEG::AVUTIL
  PRIVATE FFMPEG::SWSCALE
  PRIVATE SWRESAMPLE::SWRESAMPLE
  PRIVATE blast
  PRIVATE cxxopts
  PRIVATE ghc_filesystem
)
