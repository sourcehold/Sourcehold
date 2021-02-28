#pragma once

#include <entt/entt.hpp>

namespace Sourcehold {
    namespace ECS {
        const enum class EntityType {
            LORD,
            DEER,

            TREE_APPLE_BUD,
            TREE_APPLE_FLOWER,
            TREE_APPLE_FRUIT,
            TREE_APPLE_EMPTY,
            TREE_APPLE_STUMP,
    
            TREE_BIRCH_XL,
            TREE_BIRCH_XL_FALLING,
            TREE_BIRCH_XL_RESOURCE,
            TREE_BIRCH_L,
            TREE_BIRCH_L_FALLING,
            TREE_BIRCH_L_RESOURCE,
            TREE_BIRCH_DEAD,
            TREE_BIRCH_STUMP,

            TREE_CHESTNUT_XL,
            TREE_CHESTNUT_XL_FALLING,
            TREE_CHESTNUT_XL_RESOURCE,
            TREE_CHESTNUT_L,
            TREE_CHESTNUT_L_FALLING,
            TREE_CHESTNUT_L_RESOURCE,
            TREE_CHESTNUT_M,
            TREE_CHESTNUT_M_FALLING,
            TREE_CHESTNUT_M_RESOURCE,
            TREE_CHESTNUT_S,
            TREE_CHESTNUT_S_FALLING,
            TREE_CHESTNUT_S_RESOURCE,
            TREE_CHESTNUT_DEAD,
            TREE_CHESTNUT_STUMP,

            TREE_OAK_L,
            TREE_PINE_L,

            TREE_OAK_XL,
            TREE_PINE_XL,
            TREE_SHRUB1_GREEN,
            TREE_SHRUB1_RED,
            TREE_SHRUB2
        };

        const enum class EntityAsset {
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

        entt::registry initializeECS();
        entt::entity spawn(entt::registry &registry, EntityType type, int x, int y);
        void render(entt::registry &registry);
    }
}
