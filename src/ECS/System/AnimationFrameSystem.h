#pragma once

#include "ECS/ECS.h"
#include "ECS/System/BasicSystem.h"

namespace Sourcehold {
    namespace ECS {
        namespace System {
            class AnimationFrameSystem : public virtual BasicSystem<const EntityTypeComponent, AnimatedComponent> {
                // TODO - make proper frame index system
                double now = 0;

                public:
                AnimationFrameSystem() {};
                virtual void beforeRun() override;
                virtual void each(entt::registry &registry, EachEntityType entity) override;
            };
        }
    }
}
