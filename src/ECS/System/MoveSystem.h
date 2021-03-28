#pragma once

#include "ECS/ECS.h"
#include "ECS/System/BasicSystem.h"

namespace Sourcehold {
    namespace ECS {
        namespace System {
            class MoveSystem : public virtual BasicSystem<Component::Position, Component::MoveTarget> {
                public:
                MoveSystem() {};
                virtual void tickEntity(entt::registry &registry, EachEntityType &entity) override;
                virtual void prepareTick() override {};
            };
        }
    }
}
