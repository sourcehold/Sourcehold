#pragma once

#include <cinttypes>
#include <cstdlib>
#include <memory>

#include <GameManager.h>

#include <Events/Event.h>
#include <Events/Keyboard.h>
#include <Events/Mouse.h>

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

        class MainMenu : protected EventConsumer<Keyboard>, protected EventConsumer<Mouse>
        {
            public:
                MainMenu(std::shared_ptr<GameManager> man);
                MainMenu(const MainMenu &) = delete;
                ~MainMenu();

                int Startup();
            protected:
                void onEventReceive(Keyboard &event) override;
                void onEventReceive(Mouse &event) override;

                bool introPlaying = true;
                std::shared_ptr<GameManager> manager;
                TgxFile tgx_loading, tgx_bg1;
                BinkVideo intro;
        };
    }
}
