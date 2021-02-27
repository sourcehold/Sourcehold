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

            class RenderSystem : public virtual BasicSystem<const Component::EntityType, const Component::Position, const Component::Animation> {
                enum class EntityAsset {
                    LORD,
                    DEER,
                    TREE_APPLE,
                    TREE_BIRCH,
                    TREE_CHESTNUT,
                    TREE_OAK,
                    TREE_PINE,
                    TREE_SHRUB1,
                    TREE_SHRUB2
                };

                const ghc::filesystem::path dataRoot = GetDirectory();
                std::unordered_map<EntityAsset, GM1FileSP> EntityAssetToGMMapping {
                    { EntityAsset::DEER, GetGm1(dataRoot.string() + "gm/body_deer.gm1") },
                    { EntityAsset::LORD, GetGm1(dataRoot.string() + "gm/body_lord.gm1") },
                    { EntityAsset::TREE_APPLE, GetGm1(dataRoot.string() + "gm/tree_apple.gm1") },
                    { EntityAsset::TREE_BIRCH, GetGm1(dataRoot.string() + "gm/tree_birch.gm1") },
                    { EntityAsset::TREE_CHESTNUT, GetGm1(dataRoot.string() + "gm/Tree_Chestnut.gm1") },
                    { EntityAsset::TREE_OAK, GetGm1(dataRoot.string() + "gm/tree_oak.gm1") },
                    { EntityAsset::TREE_PINE, GetGm1(dataRoot.string() + "gm/tree_pine.gm1") },
                    { EntityAsset::TREE_SHRUB1, GetGm1(dataRoot.string() + "gm/tree_shrub1.gm1") },
                    { EntityAsset::TREE_SHRUB2, GetGm1(dataRoot.string() + "gm/tree_shrub2.gm1") }
                };

                // TODO - maybe exclude that into external config or add automatic code generation?
                std::unordered_map<EntityType, EntityAsset> EntityTypeToEntityAssetMapping;
                void _EntityTypeToEntityAssetMapping() {
                    flatMapIntoMap(
                        EntityTypeToEntityAssetMapping,
                        {
                            {
                                EntityAsset::LORD, {
                                    EntityType::LORD
                                }
                            },
                            {
                                EntityAsset::DEER, {
                                    EntityType::DEER
                                }
                            },
                            {
                                EntityAsset::TREE_SHRUB2, {
                                    EntityType::TREE_SHRUB2
                                }
                            },
                            {
                                EntityAsset::TREE_APPLE, {
                                    EntityType::TREE_APPLE_BUD,
                                    EntityType::TREE_APPLE_EMPTY,
                                    EntityType::TREE_APPLE_FLOWER,
                                    EntityType::TREE_APPLE_FRUIT,
                                    EntityType::TREE_APPLE_STUMP,
                                }
                            },
                            {
                                EntityAsset::TREE_BIRCH, {
                                    EntityType::TREE_BIRCH_L,
                                    EntityType::TREE_BIRCH_XL,
                                }
                            },
                            {
                                EntityAsset::TREE_BIRCH, {
                                    EntityType::TREE_BIRCH_L,
                                    EntityType::TREE_BIRCH_XL,
                                }
                            },
                            {
                                EntityAsset::TREE_CHESTNUT, {
                                    EntityType::TREE_CHESTNUT_L,
                                    EntityType::TREE_CHESTNUT_XL,
                                }
                            },
                            {
                                EntityAsset::TREE_OAK, {
                                    EntityType::TREE_OAK_L,
                                    EntityType::TREE_OAK_XL,
                                }
                            },
                            {
                                EntityAsset::TREE_PINE, {
                                    EntityType::TREE_PINE_L,
                                    EntityType::TREE_PINE_XL,
                                }
                            },
                            {
                                EntityAsset::TREE_SHRUB1, {
                                    EntityType::TREE_SHRUB1_RED,
                                    EntityType::TREE_SHRUB1_GREEN
                                }
                            }
                        }
                    );
                };

                void flatMapIntoMap (
                    std::unordered_map<EntityType, EntityAsset> &outMap,
                    std::unordered_map<EntityAsset, std::vector<EntityType>> maps
                ) {
                    for (auto map : maps) {
                        auto [assetEnum, entityTypes] = map;
                        for (auto entityType : entityTypes) {
                            outMap.insert_or_assign(entityType, assetEnum);
                        }
                    }
                }

                void renderUnit(Component::Position position, Component::Animation animationComponent, std::shared_ptr<Gm1File> assetFile);
                
                public:
                RenderSystem();
                virtual void tickEntity(entt::registry &registry, EachEntityType &entity) override;
                virtual void prepareTick() override {};
            };
        }
    }
}
