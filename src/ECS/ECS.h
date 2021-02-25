#pragma once

#include <entt/entt.hpp>

namespace Sourcehold {
    namespace ECS {
        enum class EntityType {
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

        // TODO - move components to own namespace        
        struct PositionComponent {
            int x;
            int y;
        };
    
        struct EntityTypeComponent {
            EntityType type;
        };

        struct AnimatedComponent {
            bool animate;
            int frameIndex;
        };

        entt::registry initializeECS();
        entt::entity spawn(entt::registry &registry, EntityType type, int x, int y);
        void render(entt::registry &registry);
    }
}
