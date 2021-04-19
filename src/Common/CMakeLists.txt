set(COMMON_SOURCE_FILES
  Shapes.h
  Rect.h
  Line.h
  Vector2.h
  Vector3.h
)

add_library(Common INTERFACE ${COMMON_SOURCE_FILES})
source_group("Source Files" FILES ${COMMON_SOURCE_FILES})

target_include_directories(Common 
  INTERFACE ${CMAKE_CURRENT_LIST_DIR}
)

