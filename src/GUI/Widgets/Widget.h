#pragma once

#include "Rendering/Shapes.h"

namespace Sourcehold {
    namespace GUI {
        using namespace Rendering;
        struct Widget {
            Widget(const Widget&) = delete;
            Widget& operator = (const Widget&) = delete;
            Widget() = default;

            virtual ~Widget() {};
            virtual void Update(Rect<int> constraints) = 0;
        };
    }
}
