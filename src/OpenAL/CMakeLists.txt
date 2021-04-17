set(OPENAL_SOURCE_FILES
  OpenAL.cpp
  AudioSource.cpp
)

add_library(OpenAL ${OPENAL_SOURCE_FILES})
source_group("Source Files" FILES ${OPENAL_SOURCE_FILES})

target_link_libraries(OpenAL
  PRIVATE ProjectOptions
  PUBLIC OpenAL::OpenAL)
