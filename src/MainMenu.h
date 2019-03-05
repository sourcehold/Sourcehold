#pragma once

#include <cinttypes>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <memory>

#include <GameManager.h>

#include <Audio/AudioSource.h>

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

        /*
         * Handles the main menu and preceding intro sequence.
         * 
         */
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

                bool introPlaying = true, startupScreen = true;
                double startTime = 0.0;
                std::shared_ptr<GameManager> manager;
                TgxFile tgx_loading, tgx_bg1, tgx_firefly, tgx_taketwo, tgx_present, tgx_logo;
                BinkVideo intro;
        };
    }
}
