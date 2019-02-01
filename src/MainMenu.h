#pragma once

#include <Config.h>
#include <StaticElement.h>
#include <RenderList.h>

namespace OpenSH
{
    namespace Rendering
    {
        class MainMenu
        {
                RenderList<StaticElement> staticElements;
            public:
                MainMenu(Context &ctx);
                ~MainMenu();
        };
    }
}
