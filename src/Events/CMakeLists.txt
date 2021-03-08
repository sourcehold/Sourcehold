add_library(Events 
  EventHandler.cpp
  Keyboard.cpp
  Mouse.cpp)

target_include_directories(Events PRIVATE ../)
target_link_libraries(Events
  PRIVATE ProjectOptions
  PRIVATE SDL2::SDL2)
