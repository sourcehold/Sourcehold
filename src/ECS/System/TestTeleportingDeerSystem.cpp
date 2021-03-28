#include "ECS/System/TestTeleportingDeerSystem.h"

namespace Sourcehold {
    namespace ECS {
        namespace System {
            void TestTeleportingDeerSystem::tickEntity(entt::registry &registry, EachEntityType &entity) {
                auto [entityRef, typeComponent, positionComponent] = entity;
                if (typeComponent.type != EntityType::WOODCUTTER) return;

                auto view = registry.view<Component::Position, Component::EntityType>();
                for(auto anyEntity: view.each()) {
                    auto [ anyEntityRef, anyPositionComponent, anyTypeComponent] = anyEntity;
                    if (anyTypeComponent.type != EntityType::TREE_CHESTNUT_XL) continue;
                    // registry.emplace_or_replace<Component::Position>(entityRef,
                    //     anyPositionComponent.x < positionComponent.x ? positionComponent.x - 1 : positionComponent.x + 1,
                    //     anyPositionComponent.y < positionComponent.y ? positionComponent.y - 1 : positionComponent.y + 1
                    // );
                    registry.emplace_or_replace<Component::MoveTarget>(entityRef,
                        false,
                        Component::Position { anyPositionComponent.x, anyPositionComponent.y }
                    );
                    break;
                }
            }
        }
    }
}
