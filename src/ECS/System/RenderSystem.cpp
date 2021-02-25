#include "ECS/System/RenderSystem.h"

namespace Sourcehold {
    namespace ECS {
        namespace System {
            RenderSystem::RenderSystem() {
                ghc::filesystem::path dataRoot = GetDirectory();
                lordFile = GetGm1(dataRoot.string() + "gm/body_lord.gm1");
                deerFile = GetGm1(dataRoot.string() + "gm/body_deer.gm1");
                appleTreeFile = GetGm1(dataRoot.string() + "gm/tree_apple.gm1");
                birchTreeFile = GetGm1(dataRoot.string() + "gm/tree_birch.gm1");
                chestnutTreeFile = GetGm1(dataRoot.string() + "gm/Tree_Chestnut.gm1");
                oakTreeFile = GetGm1(dataRoot.string() + "gm/tree_oak.gm1");
                pineTreeFile = GetGm1(dataRoot.string() + "gm/tree_pine.gm1");
                shrub1TreeFile = GetGm1(dataRoot.string() + "gm/tree_shrub1.gm1");
                shrub2TreeFile = GetGm1(dataRoot.string() + "gm/tree_shrub2.gm1");
            }

            void RenderSystem::each(EachEntityType entity) {
                const auto [entityRef, typeComponent, positionComponent, animationComponent] = entity;
                switch (typeComponent.type) {
                case EntityType::LORD: renderUnit(positionComponent, animationComponent, lordFile); break;
                case EntityType::DEER: renderUnit(positionComponent, animationComponent, deerFile); break;
                case EntityType::TREE_APPLE: renderUnit(positionComponent, animationComponent, appleTreeFile); break;
                case EntityType::TREE_BIRCH: renderUnit(positionComponent, animationComponent, birchTreeFile); break;
                case EntityType::TREE_CHESTNUT: renderUnit(positionComponent, animationComponent, chestnutTreeFile); break;
                case EntityType::TREE_OAK: renderUnit(positionComponent, animationComponent, oakTreeFile); break;
                case EntityType::TREE_PINE: renderUnit(positionComponent, animationComponent, pineTreeFile); break;
                case EntityType::TREE_SHRUB1: renderUnit(positionComponent, animationComponent, shrub1TreeFile); break;
                case EntityType::TREE_SHRUB2: renderUnit(positionComponent, animationComponent, shrub2TreeFile); break;
                }
            }
        }
    }
}
