#include "ECS/System/MoveSystem.h"

namespace Sourcehold {
    namespace ECS {
        namespace System {
            void MoveSystem::tickEntity(entt::registry &registry, EachEntityType &entity) {
                auto [ entityRef, positionComponent, moveTargetComponent ] = entity;
                
                if (moveTargetComponent.finished) return;
                
                auto [ x, y ] = positionComponent;
                
                if (x < moveTargetComponent.target.x) x++;
                if (x > moveTargetComponent.target.x) x--;
                if (y < moveTargetComponent.target.y) y++;
                if (y > moveTargetComponent.target.y) y--;
                
                registry.emplace_or_replace<Component::Position>(entityRef, x, y);

                if (x == moveTargetComponent.target.x && y == moveTargetComponent.target.y) {
                    registry.emplace_or_replace<Component::MoveTarget>(entityRef, true, moveTargetComponent.target);
                }
            }
        }
    }
}
