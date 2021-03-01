#pragma once

#include <tuple>

#include "ECS/ECS.h"
#include "ECS/System/BasicSystem.h"

#include "Utils/MultiIndexMap.h"

namespace Sourcehold {
    namespace ECS {
        namespace System {
            enum class FrameFilter {
                EVEN,
                ODD,
                NONE
            };

            struct FrameMeta {
                bool mirror;
                bool dontAnimate; // just set static frame
            };

            struct FrameData {
                int frameCount;
                int frameOffset;
                FrameMeta frameMeta;
                FrameFilter frameFilter = FrameFilter::NONE;
            };

            // Consts to make config more readable
            const int TREE_WIND_FRAMES = 25;
            const int TREE_FALLING_FRAMES = 7;
            const int SINGLE_FRAME = 1; 

            const int BIRCH_XL_RESOURCE_FRAMES = 4;
            const int BIRCH_L_RESOURCE_FRAMES = 3;

            const int CHESTNUT_XL_RESOURCE_FRAMES = 10;
            const int CHESTNUT_L_RESOURCE_FRAMES = 5;
            const int CHESTNUT_M_RESOURCE_FRAMES = 3;
            const int CHESTNUT_S_FALLING_FRAMES = 5; // special case!
            const int CHESTNUT_S_RESOURCE_FRAMES = 2;


            const FrameMeta NONE = FrameMeta { false, false };
            const FrameMeta WITH_MIRROR = FrameMeta { true, false };
            const FrameMeta DONT_ANIMATE = FrameMeta { false, true };
            const int NO_OFFSET = 0;

            // Notes:
            // 2 Birch wind animations
            enum class FrameDataGroup {
                // I wanted to call those tree groups via "tree_group_xl" but it was unwieldy for longer run
                TREE_GROUP_1,
                TREE_GROUP_2,
                TREE_GROUP_3,
                TREE_GROUP_4,

                TREE_BIRCH_XL_FALLING,
                TREE_BIRCH_XL_RESOURCE,
                TREE_BIRCH_L_FALLING,
                TREE_BIRCH_L_RESOURCE,
                TREE_BIRCH_DEAD,
                TREE_BIRCH_STUMP,

                TREE_CHESTNUT_XL_FALLING,
                TREE_CHESTNUT_XL_RESOURCE,
                TREE_CHESTNUT_L_FALLING,
                TREE_CHESTNUT_L_RESOURCE,
                TREE_CHESTNUT_M_FALLING,
                TREE_CHESTNUT_M_RESOURCE,
                TREE_CHESTNUT_S_FALLING,
                TREE_CHESTNUT_S_RESOURCE,
                TREE_CHESTNUT_DEAD,
                TREE_CHESTNUT_STUMP,

                TREE_APPLE_STUMP,

                SHRUB1_GREEN,
                SHRUB1_RED
            };

            constexpr int treeFrameOffset (int windAnimations, int fallingAnimations, int resourceFrames) {
                return windAnimations * TREE_WIND_FRAMES + fallingAnimations * TREE_FALLING_FRAMES + resourceFrames;
            }
            
            class AnimationFrameSystem : public virtual BasicSystem<const Component::EntityType, Component::Animation> {
                public:
                AnimationFrameSystem() {};
                virtual void prepareTick() override;
                virtual void tickEntity(entt::registry &registry, EachEntityType &entity) override;
                
                private:
                // TODO - make proper frame index system
                double now = 0;

                #undef _RESOURCE_OFFSET
                #define _RESOURCE_OFFSET 0
                const std::unordered_map<FrameDataGroup, FrameData> FrameDataGroupToFrameDataMapping {
                    { FrameDataGroup::TREE_GROUP_1, { TREE_WIND_FRAMES, treeFrameOffset(0, 0, 0), WITH_MIRROR } },
                    { FrameDataGroup::TREE_GROUP_2, { TREE_WIND_FRAMES, treeFrameOffset(1, 0, 0), WITH_MIRROR } },
                    { FrameDataGroup::TREE_GROUP_3, { TREE_WIND_FRAMES, treeFrameOffset(2, 0, 0), WITH_MIRROR } },
                    { FrameDataGroup::TREE_GROUP_4, { TREE_WIND_FRAMES, treeFrameOffset(3, 0, 0), WITH_MIRROR } },

                    // APPLE
                    { FrameDataGroup::TREE_APPLE_STUMP, { SINGLE_FRAME, treeFrameOffset(4, 0, 0), DONT_ANIMATE } },

                    // BIRCH
                    #undef _RESOURCE_OFFSET
                    #define _RESOURCE_OFFSET 0
                    { FrameDataGroup::TREE_BIRCH_XL_FALLING, { TREE_FALLING_FRAMES, treeFrameOffset(2, 0, _RESOURCE_OFFSET), NONE }},
                    { FrameDataGroup::TREE_BIRCH_XL_RESOURCE, { BIRCH_XL_RESOURCE_FRAMES, treeFrameOffset(2, 1, _RESOURCE_OFFSET), DONT_ANIMATE }},
                    #undef _RESOURCE_OFFSET
                    #define _RESOURCE_OFFSET BIRCH_XL_RESOURCE_FRAMES
                    { FrameDataGroup::TREE_BIRCH_L_FALLING, { TREE_FALLING_FRAMES, treeFrameOffset(2, 1, _RESOURCE_OFFSET), NONE }},
                    { FrameDataGroup::TREE_BIRCH_L_RESOURCE, { BIRCH_L_RESOURCE_FRAMES, treeFrameOffset(2, 2, _RESOURCE_OFFSET), DONT_ANIMATE }},
                    #undef _RESOURCE_OFFSET
                    #define _RESOURCE_OFFSET BIRCH_XL_RESOURCE_FRAMES + BIRCH_L_RESOURCE_FRAMES
                    { FrameDataGroup::TREE_BIRCH_DEAD, { SINGLE_FRAME, treeFrameOffset(2, 2, _RESOURCE_OFFSET), DONT_ANIMATE }},
                    #undef _RESOURCE_OFFSET
                    #define _RESOURCE_OFFSET BIRCH_XL_RESOURCE_FRAMES + BIRCH_L_RESOURCE_FRAMES + SINGLE_FRAME // SINGLE_FRAME from TREE_BIRCH_DEAD
                    { FrameDataGroup::TREE_BIRCH_STUMP, { SINGLE_FRAME, treeFrameOffset(2, 2, _RESOURCE_OFFSET), DONT_ANIMATE }},

                    // CHESTNUT
                    #undef _RESOURCE_OFFSET
                    #define _RESOURCE_OFFSET 0
                    { FrameDataGroup::TREE_CHESTNUT_XL_FALLING, { TREE_FALLING_FRAMES, treeFrameOffset(4, 0, _RESOURCE_OFFSET), NONE }},
                    { FrameDataGroup::TREE_CHESTNUT_XL_RESOURCE, { CHESTNUT_XL_RESOURCE_FRAMES, treeFrameOffset(4, 1, _RESOURCE_OFFSET), DONT_ANIMATE }},
                    #undef _RESOURCE_OFFSET
                    #define _RESOURCE_OFFSET CHESTNUT_XL_RESOURCE_FRAMES
                    { FrameDataGroup::TREE_CHESTNUT_L_FALLING, { TREE_FALLING_FRAMES, treeFrameOffset(4, 1, _RESOURCE_OFFSET), NONE }},
                    { FrameDataGroup::TREE_CHESTNUT_L_RESOURCE, { CHESTNUT_L_RESOURCE_FRAMES, treeFrameOffset(4, 2, _RESOURCE_OFFSET), DONT_ANIMATE }},
                    #undef _RESOURCE_OFFSET
                    #define _RESOURCE_OFFSET CHESTNUT_XL_RESOURCE_FRAMES + CHESTNUT_L_RESOURCE_FRAMES
                    { FrameDataGroup::TREE_CHESTNUT_M_FALLING, { TREE_FALLING_FRAMES, treeFrameOffset(4, 2, _RESOURCE_OFFSET), NONE }},
                    { FrameDataGroup::TREE_CHESTNUT_M_RESOURCE, { CHESTNUT_M_RESOURCE_FRAMES, treeFrameOffset(4, 3, _RESOURCE_OFFSET), DONT_ANIMATE }},
                    #undef _RESOURCE_OFFSET
                    #define _RESOURCE_OFFSET CHESTNUT_XL_RESOURCE_FRAMES + CHESTNUT_L_RESOURCE_FRAMES + CHESTNUT_M_RESOURCE_FRAMES
                    { FrameDataGroup::TREE_CHESTNUT_S_FALLING, { CHESTNUT_S_FALLING_FRAMES, treeFrameOffset(4, 3, _RESOURCE_OFFSET), NONE }}, // special case!
                    #undef _RESOURCE_OFFSET
                    #define _RESOURCE_OFFSET CHESTNUT_XL_RESOURCE_FRAMES + CHESTNUT_L_RESOURCE_FRAMES + CHESTNUT_M_RESOURCE_FRAMES + CHESTNUT_S_FALLING_FRAMES
                    { FrameDataGroup::TREE_CHESTNUT_S_RESOURCE, { CHESTNUT_S_RESOURCE_FRAMES, treeFrameOffset(4, 3, _RESOURCE_OFFSET), DONT_ANIMATE }},
                    #undef _RESOURCE_OFFSET
                    #define _RESOURCE_OFFSET CHESTNUT_XL_RESOURCE_FRAMES + CHESTNUT_L_RESOURCE_FRAMES + CHESTNUT_M_RESOURCE_FRAMES + CHESTNUT_S_FALLING_FRAMES + CHESTNUT_S_RESOURCE_FRAMES
                    { FrameDataGroup::TREE_CHESTNUT_DEAD, { SINGLE_FRAME, treeFrameOffset(4, 3, _RESOURCE_OFFSET), DONT_ANIMATE }},
                    #undef _RESOURCE_OFFSET
                    #define _RESOURCE_OFFSET CHESTNUT_XL_RESOURCE_FRAMES + CHESTNUT_L_RESOURCE_FRAMES + CHESTNUT_M_RESOURCE_FRAMES + CHESTNUT_S_FALLING_FRAMES + CHESTNUT_S_RESOURCE_FRAMES + SINGLE_FRAME // SINGLE_FRAME from TREE_CHESTNUT_DEAD
                    { FrameDataGroup::TREE_CHESTNUT_STUMP, { SINGLE_FRAME, treeFrameOffset(4, 3, _RESOURCE_OFFSET), DONT_ANIMATE }},

                    { FrameDataGroup::SHRUB1_GREEN, { TREE_WIND_FRAMES, NO_OFFSET, WITH_MIRROR, FrameFilter::EVEN } },
                    { FrameDataGroup::SHRUB1_RED, { TREE_WIND_FRAMES, NO_OFFSET, WITH_MIRROR, FrameFilter::ODD } },

                };

                const std::unordered_map<EntityType, FrameDataGroup> EntityTypeToFrameDataGroupMapping = Utils::createMultiIndexMap<EntityType, FrameDataGroup>({
                    { FrameDataGroup::TREE_CHESTNUT_XL_FALLING, { EntityType::TREE_CHESTNUT_XL_FALLING } },
                    { FrameDataGroup::TREE_CHESTNUT_XL_RESOURCE, { EntityType::TREE_CHESTNUT_XL_RESOURCE } },
                    { FrameDataGroup::TREE_CHESTNUT_L_FALLING, { EntityType::TREE_CHESTNUT_L_FALLING } },
                    { FrameDataGroup::TREE_CHESTNUT_L_RESOURCE, { EntityType::TREE_CHESTNUT_L_RESOURCE } },
                    { FrameDataGroup::TREE_CHESTNUT_M_FALLING, { EntityType::TREE_CHESTNUT_M_FALLING } },
                    { FrameDataGroup::TREE_CHESTNUT_M_RESOURCE, { EntityType::TREE_CHESTNUT_M_RESOURCE } },
                    { FrameDataGroup::TREE_CHESTNUT_S_FALLING, { EntityType::TREE_CHESTNUT_S_FALLING } },
                    { FrameDataGroup::TREE_CHESTNUT_S_RESOURCE, { EntityType::TREE_CHESTNUT_S_RESOURCE } },
                    { FrameDataGroup::TREE_CHESTNUT_DEAD, { EntityType::TREE_CHESTNUT_DEAD } },
                    { FrameDataGroup::TREE_CHESTNUT_STUMP, { EntityType::TREE_CHESTNUT_STUMP } },

                    { FrameDataGroup::TREE_BIRCH_XL_FALLING, { EntityType::TREE_BIRCH_XL_FALLING } },
                    { FrameDataGroup::TREE_BIRCH_XL_RESOURCE, { EntityType::TREE_BIRCH_XL_RESOURCE } },
                    { FrameDataGroup::TREE_BIRCH_L_FALLING, { EntityType::TREE_BIRCH_L_FALLING } },
                    { FrameDataGroup::TREE_BIRCH_L_RESOURCE, { EntityType::TREE_BIRCH_L_RESOURCE } },
                    { FrameDataGroup::TREE_BIRCH_DEAD, { EntityType::TREE_BIRCH_DEAD } },
                    { FrameDataGroup::TREE_BIRCH_STUMP, { EntityType::TREE_BIRCH_STUMP } },

                    { FrameDataGroup::TREE_APPLE_STUMP, { EntityType::TREE_APPLE_STUMP } },
                    {
                        FrameDataGroup::TREE_GROUP_4, {
                            EntityType::TREE_APPLE_EMPTY,
                            EntityType::TREE_CHESTNUT_S,
                        }
                    },
                    {
                        FrameDataGroup::TREE_GROUP_3, {
                            EntityType::TREE_APPLE_FRUIT,
                            EntityType::TREE_CHESTNUT_M,

                        }
                    },
                    {
                        FrameDataGroup::TREE_GROUP_2, {
                            EntityType::TREE_APPLE_FLOWER,
                            EntityType::TREE_BIRCH_L,
                            EntityType::TREE_CHESTNUT_L,
                            EntityType::TREE_OAK_L,
                            EntityType::TREE_PINE_L,
                        }
                    },
                    {
                        FrameDataGroup::TREE_GROUP_1, {
                            EntityType::TREE_APPLE_BUD,
                            EntityType::TREE_BIRCH_XL,
                            EntityType::TREE_CHESTNUT_XL,
                            EntityType::TREE_OAK_XL,
                            EntityType::TREE_PINE_XL,
                            EntityType::TREE_SHRUB2,
                        }
                    },
                    {
                        FrameDataGroup::SHRUB1_GREEN, {
                            EntityType::TREE_SHRUB1_GREEN,
                        }
                    },
                    {
                        FrameDataGroup::SHRUB1_RED, {
                            EntityType::TREE_SHRUB1_RED,
                        }
                    }
                });
            };
        }
    }
}
