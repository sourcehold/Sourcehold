# Thirdparty
# blast
add_library(blast ${CMAKE_SOURCE_DIR}/thirdparty/blast/blast.c)
set_target_properties(blast PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/thirdparty/blast)

# cxxopts
set(CXXOPTS_BUILD_TESTS OFF CACHE BOOL "Build cxxopts tests")
set(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "Build cxxopts examples")

add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/cxxopts)

# ghc filesystem
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/filesystem)

# pthread
find_package(Threads REQUIRED)

# SDL2
find_package(SDL2 REQUIRED)

# OpenAL
find_package(OpenAL REQUIRED)
AddImportedTarget(OpenAL::OpenAL ${OPENAL_LIBRARY} ${OPENAL_INCLUDE_DIR})

# EnTT
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/entt)

# FFmpeg
find_package(FFmpeg REQUIRED COMPONENTS
  AVCODEC AVFORMAT AVUTIL SWSCALE)

# swresample
find_package(Libswresample REQUIRED)
AddImportedTarget(SWRESAMPLE::SWRESAMPLE 
  ${LIBSWRESAMPLE_LIBRARIES}
  ${LIBSWRESAMPLE_INCLUDE_DIRS})


