cmake_minimum_required(VERSION 3.14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin" AND APPLE)
  message("Building on Mac OS...")
  
  if("${CMAKE_SYSTEM_NAME}" STREQUAL "")
    message("Target platform is Mac OS.")
    set(TARGET_OS_MAC_OS TRUE)
  elseif(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
    message("Target platform is iOS.")
    set(TARGET_OS_IOS TRUE)
  endif()
endif()


# Project
project(Stronghold VERSION 0.1.2)

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

if(NOT TARGET_OS_IOS)
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
  if(TARGET_OS_MAC_OS AND OPENAL_LIBRARY MATCHES "\\.framework$")
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
endif()

# AppleUtils
if(TARGET_OS_MAC_OS OR TARGET_OS_IOS)
  add_library(AppleUtils ${CMAKE_SOURCE_DIR}/apple/Common/SHPathUtils.m)
  set_target_properties(AppleUtils PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/apple/Common)
  target_link_libraries(AppleUtils
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

add_executable(${PROJECT_NAME}
  ${_source_list}
)
  
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
    foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
        string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
        set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
        set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
        set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
    endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
else()
  target_compile_options(${PROJECT_NAME} PRIVATE
    -Wno-reorder
    -pedantic-errors
    -Ofast
    -fno-fast-math)
endif()

if(TARGET_OS_IOS)
  set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO)
  set(DATA_DIR_PATH ${CMAKE_SOURCE_DIR}/data)
  set(THIRDPARTY_IOS_DIR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ios)
  
  set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.sourcehold.\${PRODUCT_NAME:identifier}"
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/apple/iOS/Supporting Files/Info.plist"
  )
  
  if(EXISTS ${DATA_DIR_PATH})
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${DATA_DIR_PATH} $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
    )
  else()
    message("'data' directroy does not exist. Skipping copy to app bundle.")
  endif()
  
  include_directories(${THIRDPARTY_IOS_DIR_PATH}/sdl2-ios/include)
  include_directories(${THIRDPARTY_IOS_DIR_PATH}/ffmpeg-ios/include)
  
  target_link_libraries(${PROJECT_NAME}
    PRIVATE AppleUtils
    PRIVATE ${THIRDPARTY_IOS_DIR_PATH}/sdl2-ios/lib/libSDL2.a
    PRIVATE ${THIRDPARTY_IOS_DIR_PATH}/ffmpeg-ios/lib/libavcodec.a
    PRIVATE ${THIRDPARTY_IOS_DIR_PATH}/ffmpeg-ios/lib/libavformat.a
    PRIVATE ${THIRDPARTY_IOS_DIR_PATH}/ffmpeg-ios/lib/libavutil.a
    PRIVATE ${THIRDPARTY_IOS_DIR_PATH}/ffmpeg-ios/lib/libswscale.a
    PRIVATE ${THIRDPARTY_IOS_DIR_PATH}/ffmpeg-ios/lib/libswresample.a
    PRIVATE blast
    PRIVATE cxxopts
    PRIVATE ghc_filesystem
    PRIVATE iconv
    PRIVATE bz2
    PRIVATE z
    PRIVATE "-framework Foundation"
    PRIVATE "-framework AVFoundation"
    PRIVATE "-framework AudioToolbox"
    PRIVATE "-framework CoreAudio"
    PRIVATE "-framework CoreGraphics"
    PRIVATE "-framework CoreHaptics"
    PRIVATE "-framework CoreMedia"
    PRIVATE "-framework CoreMotion"
    PRIVATE "-framework CoreVideo"
    PRIVATE "-framework GameController"
    PRIVATE "-framework Metal"
    PRIVATE "-framework OpenGLES"
    PRIVATE "-framework OpenAL"
    PRIVATE "-framework Security"
    PRIVATE "-framework QuartzCore"
    PRIVATE "-framework UIKit"
    PRIVATE "-framework VideoToolbox"
  )
else()
  if(TARGET_OS_MAC_OS)
     target_link_libraries(${PROJECT_NAME}
       PRIVATE AppleUtils
     )
  endif()

  target_link_libraries(${PROJECT_NAME}
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
endif()
