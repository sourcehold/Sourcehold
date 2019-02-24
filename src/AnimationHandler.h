#pragma once

#include <cinttypes>
#include <map>
#include <utility>

#include <Rendering/Texture.h>
#include <Rendering/TextureAtlas.h>

namespace Sourcehold
{
    namespace Game
    {
        class AnimationHandler
        {
                struct AnimationSlot {
                    AnimationSlot(double t, uint16_t n) :
                        timebase(t),
                        nframes(n)
                    {}
                    double timebase;
                    uint16_t nframes;
                };
                std::map<uint16_t, AnimationSlot> slots;
            public:
                AnimationHandler();
                AnimationHandler(const AnimationHandler &ani);
                ~AnimationHandler();

                void AddSlot(uint16_t slot, Rendering::TextureAtlas &animation, const std::pair<uint16_t,uint16_t> range);
                void RemoveSlot(uint16_t slot);
                uint16_t GetFrame(uint16_t slot);

                void Update();
        };
    }
}
