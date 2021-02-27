#include <entt/entt.hpp>
#include <SDL.h>

#include "GameManager.h"

#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"
#include "Rendering/Camera.h"

#include "Parsers/Gm1File.h"

#include "System/Logger.h"
#include "ECS.h"

using namespace Sourcehold::Rendering;
using namespace Sourcehold::System;
using namespace Sourcehold::Parsers;
using namespace Sourcehold::Game;

namespace Sourcehold {
    namespace ECS {
        entt::registry initializeECS () {
            entt::registry registry;
            Logger::message(System::ECS) << "Initialized ECS" << std::endl;
            return registry;
        }

        entt::entity spawn (entt::registry &registry, EntityType type, int x, int y) {
            const entt::entity entity = registry.create();
            registry.emplace<PositionComponent>(entity, x, y);
            registry.emplace<EntityTypeComponent>(entity, type);
            registry.emplace<AnimatedComponent>(entity, true, 0);

            return entity;
        }
    }
}
