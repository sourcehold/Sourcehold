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

            class RenderSystem : public virtual BasicSystem<const EntityTypeComponent, const PositionComponent, const AnimatedComponent> {
                GM1FileSP lordFile;
                GM1FileSP deerFile;
                GM1FileSP appleTreeFile;
                GM1FileSP birchTreeFile;
                GM1FileSP chestnutTreeFile;
                GM1FileSP oakTreeFile;
                GM1FileSP pineTreeFile;
                GM1FileSP shrub1TreeFile;
                GM1FileSP shrub2TreeFile;

                void renderUnit(PositionComponent position, AnimatedComponent animationComponent, std::shared_ptr<Gm1File> assetFile);

                public:
                RenderSystem();
                virtual void each(entt::registry &registry, EachEntityType entity) override;
                virtual void beforeRun() override {};
            };
        }
    }
}
