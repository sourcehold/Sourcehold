#pragma once

#include <cinttypes>

#include "GameMap.h"

namespace Sourcehold {
    namespace Game {
        class Entity {
        public:
            Entity() = default;
            Entity(const Entity&) = delete;
            ~Entity() = delete;

            virtual void Update(WorldInformation &info);
        protected:
            uint32_t _worldX, _worldY, _worldIndex;
        };
    }
}
