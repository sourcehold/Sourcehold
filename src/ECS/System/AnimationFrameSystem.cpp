#include "GameManager.h"
#include "ECS/ECS.h"
#include "ECS/System/AnimationFrameSystem.h"

namespace Sourcehold {
    namespace ECS {
        namespace System {
            // I think compiler will take care of compressing arythmetic of this place :)
            inline void calculateFrame(int &frame, int frameCount, int frameOffset, FrameMeta frameMeta) {
                // probably can be merged, but splitting this is easier for the head
                if (frameMeta.mirror) {
                    int _frameCount = frameCount - 1;
                    frame = (_frameCount - abs(frame % (2 * _frameCount) - _frameCount)); // TODO - remove first and last doubled frame
                    frame += frameOffset;
                } else {
                    frame %= frameCount;
                    frame += frameOffset;
                }
            }

            inline void calculateOddFrame(int &frame, int frameCount, int frameOffset, FrameMeta frameMeta) {
                frame *= 2;
                frame += 1;
                calculateFrame(frame, frameCount, frameOffset, frameMeta);
            }

            inline void calculateEvenFrame(int &frame, int frameCount, int frameOffset, FrameMeta frameMeta) {
                frame *= 2;
                calculateFrame(frame, frameCount, frameOffset, frameMeta);
            }

            void AnimationFrameSystem::tickEntity(entt::registry &registry, EachEntityType &entity) {
                auto [entityRef, typeComponent, animationComponent] = entity;
                if (!animationComponent.animate) return;
                long frameDependingOnTime = long(now * 15.0f);
                int frame = frameDependingOnTime % 1000;

                FrameData frameData;
                try {
                    FrameDataGroup frameDataGroup = EntityTypeToFrameDataGroupMapping.at(typeComponent.type);
                    if (frameDataGroup == FrameDataGroup::TREE_BIRCH_XL_FALLING) {
                        std::cout << "asd" << std::endl;
                    }
                    frameData = FrameDataGroupToFrameDataMapping.at(frameDataGroup);
                } catch (std::exception& e) {
                    // no config - don't use this frame system
                    return;
                }
                
                if (frameData.frameMeta.dontAnimate) {
                    frame = frameData.frameOffset + animationComponent.subanimationState;
                } else {
                    switch (frameData.frameFilter) {
                        case FrameFilter::NONE: calculateFrame(frame, frameData.frameCount, frameData.frameOffset, frameData.frameMeta); break;
                        case FrameFilter::EVEN: calculateEvenFrame(frame, frameData.frameCount, frameData.frameOffset, frameData.frameMeta); break;
                        case FrameFilter::ODD: calculateOddFrame(frame, frameData.frameCount, frameData.frameOffset, frameData.frameMeta); break;
                    }
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
