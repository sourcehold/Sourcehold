#pragma once

#include <Config.h>
#include <Rendering/StaticElement.h>
#include <Rendering/RenderList.h>

namespace OpenSH
{
    namespace Game
    {
        using namespace Rendering;
        class MainMenu
        {
                RenderList<StaticElement> staticElements;
            public:
                MainMenu(Context &ctx);
                ~MainMenu();
        };
    }
}
