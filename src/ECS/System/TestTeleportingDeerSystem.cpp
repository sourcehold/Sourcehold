#include "ECS/System/TestTeleportingDeerSystem.h"

namespace Sourcehold {
    namespace ECS {
        namespace System {
            void TestTeleportingDeerSystem::tickEntity(entt::registry &registry, EachEntityType &entity) {
                auto [entityRef, typeComponent, positionComponent] = entity;
                if (typeComponent.type != EntityType::DEER) return;

                // just a test, remove or optimize in future
                std::random_device dev;
                std::mt19937 rng(dev());
                std::uniform_int_distribution<std::mt19937::result_type> dist(0, 2);

                registry.emplace_or_replace<Component::Position>(entityRef,
                    positionComponent.x + int(dist(rng)) - 1,
                    positionComponent.y + int(dist(rng)) - 1
                );
            }
        }
    }
}
