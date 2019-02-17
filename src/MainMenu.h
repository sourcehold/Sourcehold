#pragma once

#include <Config.h>
#include <GameManager.h>

#include <Parsers/TgxFile.h>
#include <Parsers/Gm1File.h>
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
                /* Images (tgx) */
                Parsers::TgxFile tgx_loading, tgx_firefly, tgx_background;
                /* Images (gm1) */
                Parsers::Gm1File gm1_slider_bar;
                RenderList<StaticElement> staticElements;
            public:
                MainMenu(GameManager &manager);
                ~MainMenu();

                bool Start();
        };
    }
}
