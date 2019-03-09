#pragma once

#include <cinttypes>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <memory>

#include <GameManager.h>

#include <Audio/AudioSource.h>

#include <Events/Event.h>
#include <Events/Mouse.h>

#include <Parsers/TgxFile.h>
#include <Parsers/Gm1File.h>
#include <Parsers/AniFile.h>

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
         */
        class MainMenu : protected EventConsumer<Mouse>
        {
            public:
                MainMenu(std::shared_ptr<GameManager> man);
                MainMenu(const MainMenu &) = delete;
                ~MainMenu();

                int Startup();
            protected:
                void onEventReceive(Mouse &event) override;
                int EnterMainMenu();

                double startTime = 0.0;
                std::shared_ptr<GameManager> manager;
                AudioSource aud_startup, aud_chantloop;
                TgxFile tgx_loading, tgx_bg1, tgx_firefly, tgx_taketwo, tgx_present, tgx_logo;
                enum StartupEvent : uint8_t {
                    STARTUP_LOADING = 0,
                    STARTUP_FIREFLY_LOGO = 1,
                    STARTUP_TAKETWO_LOGO = 2,
                    STARTUP_PRESENT = 3,
                    STARTUP_STRONGHOLD_LOGO = 4,
                    STARTUP_INTRO = 5,
                    STARTUP_MAIN_MENU = 6
                };
                uint8_t current = 0;
                BinkVideo intro;
        };
    }
}
