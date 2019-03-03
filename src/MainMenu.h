#pragma once

#include <cinttypes>
#include <cstdlib>
#include <memory>

#include <GameManager.h>

#include <Events/EventHandler.h>
#include <Events/Keyboard.h>

#include <Parsers/TgxFile.h>
#include <Parsers/Gm1File.h>

#include <Rendering/Texture.h>
#include <Rendering/TextureAtlas.h>
#include <Rendering/BinkVideo.h>

namespace Sourcehold
{
    namespace Game
    {
        using namespace Events;
        using namespace Parsers;
        using namespace Rendering;

        class MainMenu : protected EventHandler<Keyboard>
        {
            public:
                MainMenu(std::shared_ptr<GameManager> man);
                MainMenu(const MainMenu &) = delete;
                ~MainMenu();

                int Startup();
            protected:
                std::shared_ptr<GameManager> manager;
                TgxFile tgx_loading, tgx_bg1;
                BinkVideo intro;
        };
    }
}
