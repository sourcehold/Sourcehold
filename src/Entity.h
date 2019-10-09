#pragma once

#include <cinttypes>

#include "GameMap.h"

namespace Sourcehold {
    namespace Game {
        class Entity {
        public:
            int x, y;
            
            inline int PositionToIndex(int mapDimensions) {
                return x+y*mapDimensions;
            }
        };
    }
}
