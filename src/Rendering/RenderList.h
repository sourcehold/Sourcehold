#pragma once

#include <vector>

#include <Config.h>

namespace OpenSH
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
