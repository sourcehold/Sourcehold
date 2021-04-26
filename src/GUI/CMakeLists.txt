set(GUI_SOURCE_FILES
  Credits.h
  Credits.cpp
  Dialog.h
  Dialog.cpp
  Ingame.h
  Ingame.cpp
  Layout.h
  Layout.cpp
  MainMenu.h
  MainMenu.cpp
  MenuUtils.h
  MenuUtils.cpp
  NarrScreen.h
  NarrScreen.cpp
  UIState.h
)
set(GUI_WIDGETS_SOURCE_FILES
  Widgets/Button.h
  Widgets/Button.cpp
  Widgets/Container.h
  Widgets/Container.cpp
  Widgets/LineEdit.h
  Widgets/LineEdit.cpp
  Widgets/StaticElement.h
  Widgets/StaticElement.cpp
  Widgets/Table.h
  Widgets/Table.cpp
  Widgets/Widget.h
)

add_library(GUI
  ${GUI_SOURCE_FILES}
  ${GUI_WIDGETS_SOURCE_FILES}
)
source_group("Source Files" FILES ${GUI_SOURCE_FILES})
source_group("Source Files\\Widgets" FILES ${GUI_WIDGETS_SOURCE_FILES})

target_include_directories(GUI PRIVATE ../)
target_link_libraries(GUI
  PRIVATE ProjectOptions
  PRIVATE System
  PRIVATE SDL2::SDL2
  # GUI includes World.h .. fix this !
  # GUI doesnt use EnTT!
  PRIVATE EnTT::EnTT
  PRIVATE Audio
  PUBLIC Common
)

