cmake_minimum_required(VERSION 3.0)

# Project
project(Stronghold)
set(Stronghold_VERSION, 0.1.2)


include(cmake/Preamble.cmake)

add_subdirectory(macos)

include_directories(src)

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
  PRIVATE EnTT::EnTT
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
