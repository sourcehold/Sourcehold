#include "ECS/System/RenderSystem.h"

namespace Sourcehold {
    namespace ECS {
        namespace System {
            RenderSystem::RenderSystem() {}

            void RenderSystem::tickEntity(entt::registry &registry, EachEntityType &entity) {
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

                int px = x * 30 - cam.positionX;
                int py = y * 15 - cam.positionY;

                SDL_Rect r = assetFile->GetTextureAtlas()->Get(animationComponent.animate ? animationComponent.frameIndex : 0); // TODO: index
                Rendering::Render(*assetFile->GetTextureAtlas(), px, py, &r);
            }
        }
    }
}
