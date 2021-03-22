#pragma once

#include "ECS/ECS.h"

namespace Sourcehold {
namespace ECS {
namespace Component {
struct Position {
  int x;
  int y;
};

struct EntityType {
  ECS::EntityType type;
};

struct Animation {
  bool animate;
  // frameIndex - Replaced by AnimationFrameSystem to be passed to RenderSystem
  int frameIndex;
  // subanimationState - Replaced by subsystems to be utilized by
  // AnimationFrameSystem in order to change frame during "dontAnimate"
  // animation For example - felt tree is a "tree resource" and it has couple of
  // "harvest" states - log is becoming smaller and smaller the more wood is
  // harvested.
  int subanimationState;
};
}  // namespace Component
}  // namespace ECS
}  // namespace Sourcehold
