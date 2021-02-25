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
        void update (entt::registry &registry) {
        }

        entt::registry initializeECS () {
            entt::registry registry;
            Logger::message(System::ECS) << "Initialized ECS" << std::endl;

            return registry;
        }

        bool isTreeType (EntityType type) {
            switch (type) {
                case EntityType::TREE_APPLE:
                case EntityType::TREE_BIRCH:
                case EntityType::TREE_CHESTNUT:
                case EntityType::TREE_OAK:
                case EntityType::TREE_PINE:
                case EntityType::TREE_SHRUB1:
                case EntityType::TREE_SHRUB2:
                    return true;
                default:
                    return false;
            }
        }
        
        void renderUnit (PositionComponent position, AnimatedComponent animationComponent, std::shared_ptr<Gm1File> assetFile) {
            Camera& cam = Camera::instance();
            auto [x, y] = position;

            int px = x * 30 - cam.positionX;
            int py = y * 15 - cam.positionY;

            SDL_Rect r = assetFile->GetTextureAtlas()->Get(animationComponent.animate ? animationComponent.frameIndex : 0); // TODO: index
            Rendering::Render(*assetFile->GetTextureAtlas(), px, py, &r);
        }

        void updateAnimationFrameIndexes (entt::registry &registry, double now) {
            auto view = registry.view<const EntityTypeComponent, AnimatedComponent>();
            
            for(auto [entity, typeComponent, animationComponent]: view.each()) {
                animationComponent.frameIndex = (24 - abs(int(now * 15.0f) % (2 * 24) - 24));
            }
        }

        entt::entity spawn (entt::registry &registry, EntityType type, int x, int y) {
            const entt::entity entity = registry.create();
            registry.emplace<PositionComponent>(entity, x, y);
            registry.emplace<EntityTypeComponent>(entity, type);

            if (isTreeType(type)) registry.emplace<AnimatedComponent>(entity, true, 0);
            else registry.emplace<AnimatedComponent>(entity, false, 0);

            return entity;
        }
    }
}
