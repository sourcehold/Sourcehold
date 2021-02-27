#pragma once

#include "ECS/ECS.h"

namespace Sourcehold {
    namespace ECS {
        namespace Component {
            struct Position {
                int x;
                int y;
            };
        
            struct EntityType {
                ECS::EntityType type;
            };

            struct Animation {
                bool animate;
                int frameIndex;
            };
        }
    }
}
