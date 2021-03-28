add_library(ECS
  ECS.cpp
  Manager.cpp
  System/BasicSystem.cpp
  System/RenderSystem.cpp
  System/AnimationFrameSystem.cpp
  System/MoveSystem.cpp
  System/TestTeleportingDeerSystem.cpp
)

target_include_directories(ECS PRIVATE ../)
target_link_libraries(ECS 
  PRIVATE ProjectOptions
  PRIVATE System
  PUBLIC EnTT::EnTT
  PRIVATE SDL2::SDL2
)

