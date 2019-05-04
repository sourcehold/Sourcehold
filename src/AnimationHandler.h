#pragma once

#include <cinttypes>
#include <map>
#include <utility>

#include <SDL.h>

namespace Sourcehold
{
    namespace Game
    {
        class AnimationHandler
        {
                struct AnimationSlot {
                    enum Flags : uint8_t {
                        NONE            = 0b00000000,
                        LOOPING         = 0b10000000,
                        REVERSE         = 0b01000000,
                        ONESHOT         = 0b00100000,
                    } flags;

                    Uint32 timebase;
                    uint16_t nframes;

                    AnimationSlot(Uint32 t, uint16_t n, Flags f) {
                        timebase = t;
                        nframes = n;
                        flags = f;
                    }
                };
                double localTime;
                std::map<uint16_t, AnimationSlot> slots;
            public:
                AnimationHandler();
                AnimationHandler(const AnimationHandler &ani);
                ~AnimationHandler();

                uint16_t AddSlot(const std::pair<uint16_t,uint16_t> range);
                void RemoveSlot(uint16_t slot);
                uint16_t GetFrame(uint16_t slot);

                void Update();
        };
    }
}
