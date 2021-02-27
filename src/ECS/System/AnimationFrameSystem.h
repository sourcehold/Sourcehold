#pragma once

#include "ECS/ECS.h"
#include "ECS/System/BasicSystem.h"

namespace Sourcehold {
    namespace ECS {
        namespace System {
            class AnimationFrameSystem : public virtual BasicSystem<const Component::EntityType, Component::Animation> {
                // TODO - make proper frame index system
                double now = 0;

                public:
                AnimationFrameSystem() {};
                virtual void prepareTick() override;
                virtual void tickEntity(entt::registry &registry, EachEntityType &entity) override;
            };
        }
    }
}
