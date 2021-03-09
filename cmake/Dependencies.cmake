# Thirdparty
# blast
add_library(blast ${CMAKE_SOURCE_DIR}/thirdparty/blast/blast.c)
target_include_directories(blast
  PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/blast
)

# cxxopts
set(CXXOPTS_BUILD_TESTS OFF CACHE BOOL "Build cxxopts tests")
set(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "Build cxxopts examples")

add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/cxxopts)

# ghc filesystem
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/filesystem)

# pthread
find_package(Threads REQUIRED)

# EnTT
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/entt)

# OpenAL
find_package(OpenAL REQUIRED)
AddImportedTarget(OpenAL::OpenAL
  ${OPENAL_LIBRARY}
  ${OPENAL_INCLUDE_DIR}
)

if(TARGET_OS_IOS)
  set(THIRDPARTY_IOS_DIR_PATH ${CMAKE_SOURCE_DIR}/thirdparty/ios)
  
  # SDL2
  set(SDL2_IOS_DIR_PATH ${THIRDPARTY_IOS_DIR_PATH}/sdl2-ios)
    
  AddImportedTarget(SDL2::SDL2
    ${SDL2_IOS_DIR_PATH}/lib/libSDL2.a
    ${SDL2_IOS_DIR_PATH}/include
  )
 
  # FFmpeg
  set(FFMPEG_IOS_DIR_PATH ${THIRDPARTY_IOS_DIR_PATH}/ffmpeg-ios)
  set(FFMPEG_IOS_INCLUDE_DIR_PATH ${FFMPEG_IOS_DIR_PATH}/include)
  set(FFMPEG_IOS_LIB_DIR_PATH ${FFMPEG_IOS_DIR_PATH}/lib)
  
  AddImportedTarget(FFMPEG::AVCODEC
    ${FFMPEG_IOS_LIB_DIR_PATH}/libavcodec.a
    ${FFMPEG_IOS_INCLUDE_DIR_PATH}
  )
    
  AddImportedTarget(FFMPEG::AVFORMAT
    ${FFMPEG_IOS_LIB_DIR_PATH}/libavformat.a
    ${FFMPEG_IOS_INCLUDE_DIR_PATH}
  )
 
  AddImportedTarget(FFMPEG::AVUTIL
    ${FFMPEG_IOS_LIB_DIR_PATH}/libavutil.a
    ${FFMPEG_IOS_INCLUDE_DIR_PATH}
  )
    
  AddImportedTarget(FFMPEG::SWSCALE
    ${FFMPEG_IOS_LIB_DIR_PATH}/libswscale.a
    ${FFMPEG_IOS_INCLUDE_DIR_PATH}
  )
 
  # swresample
  AddImportedTarget(SWRESAMPLE::SWRESAMPLE
    ${FFMPEG_IOS_LIB_DIR_PATH}/libswresample.a
    ${FFMPEG_IOS_INCLUDE_DIR_PATH}
  )
  
else()

# SDL2
find_package(SDL2 REQUIRED)

# FFmpeg
find_package(FFmpeg REQUIRED COMPONENTS
  AVCODEC
  AVFORMAT
  AVUTIL
  SWSCALE
)

# swresample
find_package(Libswresample REQUIRED)
AddImportedTarget(SWRESAMPLE::SWRESAMPLE 
  ${LIBSWRESAMPLE_LIBRARIES}
  ${LIBSWRESAMPLE_INCLUDE_DIRS}
)

endif() # TARGET_OS_IOS
