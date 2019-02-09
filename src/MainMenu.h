#pragma once

#include <Config.h>
#include <GameManager.h>

#include <Parsers/TgxFile.h>
#include <Parsers/BinkVideo.h>
#include <Parsers/VolumeTxt.h>

#include <Rendering/StaticElement.h>
#include <Rendering/RenderList.h>

/* Handle main menu and preceding intro */
namespace Sourcehold
{
    namespace Game
    {
        using namespace Rendering;
        class MainMenu : public GameManager
        {
                /* Images */
                Parsers::TgxFile tgx_loading, tgx_firefly, tgx_background;
                RenderList<StaticElement> staticElements;
            public:
                MainMenu(GameManager &manager);
                ~MainMenu();

                bool Start();
        };
    }
}
