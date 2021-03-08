add_library(GUI
  Credits.cpp
  Dialog.cpp
  Ingame.cpp
  Layout.cpp
  MainMenu.cpp
  MenuUtils.cpp
  NarrScreen.cpp
  Widgets/Button.cpp
  Widgets/Container.cpp
  Widgets/LineEdit.cpp
  Widgets/StaticElement.cpp
  Widgets/Table.cpp
)

target_include_directories(GUI PRIVATE ../)
target_compile_features(GUI PRIVATE cxx_std_11)
target_link_libraries(GUI
  PRIVATE System
  PRIVATE SDL2::SDL2
  # GUI includes World.h .. fix this !
  # GUI doesnt use EnTT!
  PRIVATE EnTT::EnTT
  PRIVATE Audio)

