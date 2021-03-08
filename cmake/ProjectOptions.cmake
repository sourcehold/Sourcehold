add_library(ProjectOptions INTERFACE)
target_compile_features(ProjectOptions INTERFACE cxx_std_11)

if(MSVC)
  target_compile_options(ProjectOptions INTERFACE -D_CRT_SECURE_NO_WARNINGS)
  foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR} )
  endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
else()
  target_compile_options(ProjectOptions INTERFACE
    -Wno-reorder
    -pedantic-errors
    -Ofast
    -fno-fast-math)
endif()


# TODO(seidl):
#add warnings and linting

