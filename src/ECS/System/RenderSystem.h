#pragma once

#include "ECS/ECS.h"
#include "ECS/System/BasicSystem.h"

#include "GameManager.h"

#include <SDL.h>


#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"
#include "Rendering/Camera.h"

#include "Parsers/Gm1File.h"

using namespace Sourcehold::Rendering;
using namespace Sourcehold::System;
using namespace Sourcehold::Parsers;
using namespace Sourcehold::Game;

namespace Sourcehold {
    namespace ECS {
        namespace System {
            using namespace Game;

            class RenderSystem : public BasicSystem<const EntityTypeComponent, const PositionComponent, const AnimatedComponent> {
                GM1FileSP lordFile;
                GM1FileSP deerFile;
                GM1FileSP appleTreeFile;
                GM1FileSP birchTreeFile;
                GM1FileSP chestnutTreeFile;
                GM1FileSP oakTreeFile;
                GM1FileSP pineTreeFile;
                GM1FileSP shrub1TreeFile;
                GM1FileSP shrub2TreeFile;

                public:
                RenderSystem();
                virtual void each(EachEntityType entity) override;

                private:
                void renderUnit (PositionComponent position, AnimatedComponent animationComponent, std::shared_ptr<Gm1File> assetFile) {
                    Camera& cam = Camera::instance();
                    auto [x, y] = position;

                    int px = x * 30 - cam.positionX;
                    int py = y * 15 - cam.positionY;

                    SDL_Rect r = assetFile->GetTextureAtlas()->Get(animationComponent.animate ? animationComponent.frameIndex * 2 : 0); // TODO: index
                    Rendering::Render(*assetFile->GetTextureAtlas(), px, py, &r);
                }
            };
        }
    }
}
