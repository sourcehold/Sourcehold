#include "GameManager.h"
#include "ECS/ECS.h"
#include "ECS/System/AnimationFrameSystem.h"

namespace Sourcehold {
    namespace ECS {
        const int TREE_WIND_FRAMES = 25;
        const int TREE_SIZE_XL_FRAME_OFFSET = 0 * TREE_WIND_FRAMES;
        const int TREE_SIZE_L_FRAME_OFFSET = 1 * TREE_WIND_FRAMES;

        // TODO:
        // Adult, big, etc... has been changed to XL, L...
        // Note for further dev of animation systems
        // Trees don't have every state - f.e. Birch has only Adult and Big state.
        // Wind animation frame count for trees looks like it is constant for each tree - 25
        // Falling trees last frames looks like it is wind animation - needs investigating
        // "Tree resource" state is a state when a tree is down and ready for harvesting, it has different
        // states for different tree types
        // enum class TREE_STATES {
        //     ADULT, BIG, MEDIUM, SMALL,
        //     ADULT_FALLING, ADULT_RESOURCE,
        //     BIG_FALLING, BIG_RESOURCE,
        //     MEDIUM_FALLING, MEDIUM_RESOURCE,
        //     SMALL_FALLING, SMALL_RESOURCE,
        //     DEAD, STUMP
        // };
        // const int TREE_FRAMES_ADULT = 25;
        // const int TREE_FRAMES_BIG = 25;
        // const int TREE_FRAMES_MEDIUM = 25;
        // const int TREE_FRAMES_SMALL = 25;
        // const int TREE_FRAMES_ADULT_FALLING = 7;
        // const int TREE_FRAMES_ADULT_RESOURCE = 10;
        // const int TREE_FRAMES_BIG_FALLING = 8;
        // const int TREE_FRAMES_BIG_RESOURCE = 4;
        // const int TREE_FRAMES_MEDIUM_FALLING = 8;
        // const int TREE_FRAMES_MEDIUM_RESOURCE = 2;
        // const int TREE_FRAMES_SMALL_FALLING = 6;
        // const int TREE_FRAMES_SMALL_RESOURCE = 1;
        // const int TREE_FRAMES_DEAD = 1;
        // const int TREE_FRAMES_STUMP = 1;

        namespace System {
            enum class FrameFilter {
                EVEN,
                ODD,
                NONE
            };
            // I think compiler will take care of compressing arythmetic of this place :)
            inline void calculateFrame(int &frame, int frameCount, int frameOffset) {
                frame %= frameCount;
                frame += frameOffset;
            }
            inline void calculateOddFrame(int &frame, int frameCount, int frameOffset) {
                frame *= 2;
                frame += 1;
                calculateFrame(frame, frameCount, frameOffset);
            }
            inline void calculateEvenFrame(int &frame, int frameCount, int frameOffset) {
                frame *= 2;
                calculateFrame(frame, frameCount, frameOffset);
            }

            void AnimationFrameSystem::tickEntity(entt::registry &registry, EachEntityType &entity) {
                auto [entityRef, typeComponent, animationComponent] = entity;
                if (!animationComponent.animate) return;
                long frameDependingOnTime = long(now * 15.0f);
                // just reduce size, no animation has 1k frames in the base game
                // leaving as int, because I suspect modding in future and RAM is cheap novadays
                int frame = frameDependingOnTime % 1000; 


                // TODO - finish that
                switch(typeComponent.type) {
                case EntityType::TREE_BIRCH_L:
                case EntityType::TREE_CHESTNUT_L:
                case EntityType::TREE_OAK_L:
                case EntityType::TREE_PINE_L: calculateFrame(frame, TREE_WIND_FRAMES, TREE_SIZE_L_FRAME_OFFSET); break;

                case EntityType::TREE_BIRCH_XL:
                case EntityType::TREE_CHESTNUT_XL:
                case EntityType::TREE_OAK_XL:
                case EntityType::TREE_PINE_XL:
                    frame %= TREE_WIND_FRAMES;
                    frame += TREE_SIZE_XL_FRAME_OFFSET;
                    break;
                case EntityType::TREE_SHRUB2:
                    // Note - excluded shrub2 from XL trees to make it more copy-paste friendly 
                    frame %= TREE_WIND_FRAMES;
                    break;
                case EntityType::TREE_SHRUB1_GREEN:
                    // Green bush frames are even frames of shrub1
                    frame *= 2;
                    frame %= TREE_WIND_FRAMES;
                    break;
                case EntityType::TREE_SHRUB1_RED:
                    // Red bush frames are odd frames of shrub1
                    frame *= 2;
                    frame += 1;
                    frame %= TREE_WIND_FRAMES;
                    break;
                }

                registry.emplace_or_replace<Component::Animation>(entityRef,
                    true,
                    frame
                );
            }

            void AnimationFrameSystem::prepareTick() {
                now = Game::GetTime();
            }
        }
    }
}
