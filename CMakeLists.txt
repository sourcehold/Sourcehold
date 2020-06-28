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
include_directories(thirdparty/)
include_directories(thirdparty/cxxopts)
include_directories(thirdparty/blast/)
include_directories(thirdparty/duktape/)
include_directories(thirdparty/cxxopts/include/)
include_directories(thirdparty/filesystem/include/)

if (MSVC)
  set( SDL2_ROOT_DIR thirdparty/SDL2-2.0.10)
endif()

# pthread
find_package (Threads)

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

# OpenAL
find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})


if (MSVC)
	include_directories(thirdparty/ffmpeg/include/)
	set (FFMPEG_LIBS_DIR ${CMAKE_SOURCE_DIR}/thirdparty/ffmpeg/lib)
	set (FFMPEG_LIBS_DLL_DIR thirdparty/ffmpeg/bin)
	set (SWSCALE_LIBRARIES ${FFMPEG_LIBS_DIR}/swscale.lib
												 ${FFMPEG_LIBS_DIR}/avcodec.lib
												 ${FFMPEG_LIBS_DIR}/avdevice.lib
												 ${FFMPEG_LIBS_DIR}/avfilter.lib
												 ${FFMPEG_LIBS_DIR}/avformat.lib
												 ${FFMPEG_LIBS_DIR}/avutil.lib
												 ${FFMPEG_LIBS_DIR}/swresample.lib)
else()
	# FFmpeg
	find_package(FFmpeg REQUIRED)
	include_directories(${FFMPEG_INCLUDE_DIRS})
	include_directories(${SWSCALE_INCLUDE_DIRS})

	# swresample
	find_package(Libswresample REQUIRED)
	include_directories( ${LIBSWRESAMPLE_INCLUDE_DIRS})
endif()



# Include sources / headers
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp thirdparty/blast/blast.c thirdparty/duktape/duktape.c)
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
		string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
		set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
		set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
		set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
	endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
else()
	add_definitions(-Wno-reorder -pedantic-errors -Ofast -Wall -Wextra -fno-fast-math)
endif()

add_executable(Stronghold ${SOURCES} ${HEADERS})
set_target_properties(Stronghold PROPERTIES
 	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

if (MSVC)
	add_custom_command(TARGET Stronghold POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/${SDL2_ROOT_DIR}/lib/x64/SDL2.dll" "${CMAKE_BINARY_DIR}/SDL2.dll")
	add_custom_command(TARGET Stronghold POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/${FFMPEG_LIBS_DLL_DIR}/swscale-5.dll" "${CMAKE_BINARY_DIR}/swscale-5.dll")
  add_custom_command(TARGET Stronghold POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/${FFMPEG_LIBS_DLL_DIR}/avcodec-58.dll" "${CMAKE_BINARY_DIR}/avcodec-58.dll")
  add_custom_command(TARGET Stronghold POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/${FFMPEG_LIBS_DLL_DIR}/avformat-58.dll" "${CMAKE_BINARY_DIR}/avformat-58.dll")
  add_custom_command(TARGET Stronghold POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/${FFMPEG_LIBS_DLL_DIR}/avutil-56.dll" "${CMAKE_BINARY_DIR}/avutil-56.dll")
  add_custom_command(TARGET Stronghold POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/${FFMPEG_LIBS_DLL_DIR}/swresample-3.dll" "${CMAKE_BINARY_DIR}/swresample-3.dll")
endif()

target_link_libraries(
	Stronghold
	${SDL2_LIBRARY}
	${OPENAL_LIBRARY}
	${FFMPEG_LIBRARIES}
	${SWSCALE_LIBRARIES}
	${LIBSWRESAMPLE_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)
