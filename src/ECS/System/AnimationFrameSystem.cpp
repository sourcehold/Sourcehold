#include "GameManager.h"
#include "ECS/ECS.h"
#include "ECS/System/AnimationFrameSystem.h"

namespace Sourcehold {
    namespace ECS {
        namespace System {
            void AnimationFrameSystem::each(entt::registry &registry, EachEntityType entity) {
                auto [entityRef, typeComponent, animationComponent] = entity;
                if (!animationComponent.animate) return;

                registry.emplace_or_replace<ECS::AnimatedComponent>(entityRef,
                    true,
                    (24 - abs(int(now * 15.0f) % (2 * 24) - 24)
                ));
            }

            void AnimationFrameSystem::beforeRun() {
                now = Game::GetTime();
            }
        }
    }
}
