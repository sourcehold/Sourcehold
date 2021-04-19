set(ECS_SOURCE_FILES
  Component/Component.h
  ECS.h
  ECS.cpp
  Manager.h
  Manager.cpp
  System/BasicSystem.h
  System/BasicSystem.cpp
  System/RenderSystem.h
  System/RenderSystem.cpp
  System/AnimationFrameSystem.h
  System/AnimationFrameSystem.cpp
  System/TestTeleportingDeerSystem.h
  System/TestTeleportingDeerSystem.cpp
)

add_library(ECS ${ECS_SOURCE_FILES})
source_group("Source Files" FILES ${ECS_SOURCE_FILES})

target_include_directories(ECS PRIVATE ../)
target_link_libraries(ECS 
  PRIVATE ProjectOptions
  PRIVATE System
  PUBLIC EnTT::EnTT
  PUBLIC Common
  PRIVATE SDL2::SDL2
)

