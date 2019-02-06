#pragma once

#include <vector>

#include <Config.h>

namespace Sourcehold
{
    namespace Rendering
    {
        template<class T>
        class RenderList : std::vector<T>
        {
            public:
                RenderList() = default;
                ~RenderList() = default;
        };
    }
}
