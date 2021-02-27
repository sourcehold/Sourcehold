#include <entt/entt.hpp>
#include <SDL.h>

#include "GameManager.h"

#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"
#include "Rendering/Camera.h"

#include "Parsers/Gm1File.h"

#include "System/Logger.h"

#include "ECS.h"
#include "ECS/Component/Component.h"

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
            registry.emplace<Component::Position>(entity, x, y);
            registry.emplace<Component::EntityType>(entity, type);
            registry.emplace<Component::Animation>(entity, true, 0);

            return entity;
        }
    }
}
