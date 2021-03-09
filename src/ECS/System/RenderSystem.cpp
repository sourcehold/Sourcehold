#include "ECS/System/RenderSystem.h"

namespace Sourcehold {
    namespace ECS {
        namespace System {
            RenderSystem::RenderSystem() {}

            void RenderSystem::tickEntity([[maybe_unused]] entt::registry &registry, EachEntityType &entity) {
                const auto [entityRef, typeComponent, positionComponent, animationComponent] = entity;
                GM1FileSP assetFile;
                try {
                    EntityAsset entityAsset = EntityTypeToEntityAssetMapping.at(typeComponent.type);
                    assetFile = EntityAssetToGMMapping.at(entityAsset);
                } catch (std::exception& e) {
                    // no file or mapping - don't render
                    return;
                }
                renderUnit(positionComponent, animationComponent, assetFile);
            }

            void RenderSystem::renderUnit(Component::Position position, Component::Animation animationComponent, std::shared_ptr<Gm1File> assetFile) {
                Camera& cam = Camera::instance();
                auto [x, y] = position;

                int px = x * 30 - cam.pos_x_;
                int py = y * 15 - cam.pos_y_;

                SDL_Rect r = assetFile->GetTextureAtlas()->Get(animationComponent.animate ? static_cast<Uint32>(animationComponent.frameIndex) : 0); // TODO: index
                Rendering::Render(*assetFile->GetTextureAtlas(), px, py, &r);
            }
        }
    }
}
