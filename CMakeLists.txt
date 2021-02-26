cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin" AND APPLE)
  message("Building for Mac OS...")
  set(MAC_OS TRUE)
endif()

# include cmake utilities
include(AddImportedTarget)
include(PreventInSourceBuilds)
include(CompilerWarnings)
include(Sanitizers)


set(ENABLE_CLANG_TIDY ON CACHE BOOL "CLANG TIDY")
set(ENABLE_INCLUDE_WHAT_YOU_USE ON CACHE BOOL "INCLUDE WHAT YOU USE")

include(StaticAnalyzers)


# Project
project(Stronghold)
set(Stronghold_VERSION, 0.1.2)

# options
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_11)
enable_sanitizers(project_options)

# warnings
add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)


# Thirdparty

# blast
add_library(blast thirdparty/blast/blast.c)
set_target_properties(blast PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/thirdparty/blast)



# cxxopts
set(CXXOPTS_BUILD_TESTS OFF CACHE BOOL "BUILD CXXOPTS TESTS")
set(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "BUILD CXXOPTS EXAMPLES")
add_subdirectory(thirdparty/cxxopts)


# ghc filesystem
add_subdirectory(thirdparty/filesystem)


# pthread
find_package(Threads REQUIRED)


# SDL2
find_package(SDL2 REQUIRED)


# OpenAL
find_package(OpenAL REQUIRED)
AddImportedTarget(OpenAL::OpenAL ${OPENAL_LIBRARY} ${OPENAL_INCLUDE_DIR})


# FFmpeg
find_package(FFmpeg REQUIRED COMPONENTS
  AVCODEC AVFORMAT AVUTIL SWSCALE)


# swresample
find_package(Libswresample REQUIRED)
AddImportedTarget(SWRESAMPLE::SWRESAMPLE ${LIBSWRESAMPLE_LIBRARIES} ${LIBSWRESAMPLE_INCLUDE_DIRS})


# MacOSUtils
# TODO(apodrugin): 
# If this gets too big, create a new CMakeLists.txt in the macos directory
# then just: include(macos)
if(MAC_OS)
  add_library(MacOSUtils macos/src/PathUtils.m)
  target_include_directories(MacOSUtils PUBLIC ${CMAKE_SOURCE_DIR}/macos/src)
  target_link_libraries(MacOSUtils
    PRIVATE "-framework Foundation")
endif()

# Include sources / headers
file(
  GLOB_RECURSE _source_list
  LIST_DIRECTORIES false
  "${CMAKE_SOURCE_DIR}/src/*.cpp*"
  "${CMAKE_SOURCE_DIR}/src/*.h*"
)

#Engine
add_subdirectory(src/Engine)

add_executable(Sourcehold 
  src/Main.cpp
  src/Commandline.cpp
  src/StartGame.cpp)

target_compile_features(Sourcehold PRIVATE cxx_std_11)
target_include_directories(Sourcehold PRIVATE src)

target_link_libraries(Sourcehold
  PRIVATE Engine
  PRIVATE cxxopts
  PRIVATE project_warnings 
  PRIVATE project_options
  )



#foreach(_source IN ITEMS ${_source_list})
#  get_filename_component(_source_path "${_source}" PATH)
#  file(RELATIVE_PATH _source_path_rel "${CMAKE_SOURCE_DIR}/src/" "${_source_path}")
#  string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
#  source_group("${_group_path}" FILES "${_source}")
#endforeach()
#
#add_executable(Stronghold ${_source_list})
#
#if(MSVC)
#  target_compile_options(Stronghold PRIVATE -D_CRT_SECURE_NO_WARNINGS)
#  foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
#    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
#    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
#    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
#    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
#  endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
#else()
#  target_compile_options(Stronghold PRIVATE
#                                    -Wno-reorder
#                                    -pedantic-errors
#                                    -Ofast
#                                    -fno-fast-math)
#endif()
#
#set_target_properties(Stronghold PROPERTIES
#  CXX_STANDARD 11
#  CXX_STANDARD_REQUIRED YES
#  CXX_EXTENSIONS NO
#)
#
#if(MAC_OS)
#  target_link_libraries(Stronghold PRIVATE MacOSUtils)
#endif()
#
#target_link_libraries(Stronghold
#  PRIVATE Threads::Threads
#  PRIVATE SDL2::SDL2
#  PRIVATE OpenAL::OpenAL
#  PRIVATE FFMPEG::AVCODEC
#  PRIVATE FFMPEG::AVFORMAT
#  PRIVATE FFMPEG::AVUTIL
#  PRIVATE FFMPEG::SWSCALE
#  PRIVATE SWRESAMPLE::SWRESAMPLE
#  PRIVATE blast
#  PRIVATE cxxopts
#  PRIVATE ghc_filesystem
#)
