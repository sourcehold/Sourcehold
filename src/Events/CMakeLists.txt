set(EVENTS_SOURCE_FILES
  Event.h
  EventHandler.h
  EventHandler.cpp
  Keyboard.h
  Keyboard.cpp
  Mouse.h
  Mouse.cpp
  Touch.h
)

add_library(Events ${EVENTS_SOURCE_FILES})
source_group("Source Files" FILES ${EVENTS_SOURCE_FILES})

target_include_directories(Events PRIVATE ../)
target_link_libraries(Events
  PRIVATE ProjectOptions
  PRIVATE SDL2::SDL2
)
