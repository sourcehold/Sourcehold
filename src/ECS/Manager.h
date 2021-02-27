#ifndef ECS__MANAGER_H_
#define ECS__MANAGER_H_

#include "ECS/System/RenderSystem.h"
#include "ECS/System/AnimationFrameSystem.h"
#include "ECS/System/TestTeleportingDeerSystem.h"

namespace Sourcehold {
    namespace ECS {
        class Manager {
            public:
            // Meyer Singleton
            static Manager& GetInstance() {
                static Manager manager;
                return manager;
            }

            static void Render(entt::registry &registry) {
                GetInstance()._RenderSystem.run(registry);
            }

            static void Update(entt::registry &registry) {
                GetInstance()._AnimationFrameSystem.run(registry);
                GetInstance()._TestTeleportingDeerSystem.run(registry);
            }

            private:
            Manager() = default;
            Manager(const Manager& other) = delete;
            Manager& operator=(const Manager& other) = delete;
            Manager& operator=(Manager&& other) = delete;
            ~Manager() = default;
            
            ECS::System::RenderSystem _RenderSystem = ECS::System::RenderSystem();
            ECS::System::AnimationFrameSystem _AnimationFrameSystem = ECS::System::AnimationFrameSystem();
            ECS::System::TestTeleportingDeerSystem _TestTeleportingDeerSystem = ECS::System::TestTeleportingDeerSystem();
        };
    }
}

#endif  // ECS__MANAGER_H_