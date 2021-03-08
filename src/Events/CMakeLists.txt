add_library(Events 
  EventHandler.cpp
  Keyboard.cpp
  Mouse.cpp)

target_include_directories(Events PRIVATE ../)
target_compile_features(Events PRIVATE cxx_std_11)
target_link_libraries(Events
  PRIVATE SDL2::SDL2)
