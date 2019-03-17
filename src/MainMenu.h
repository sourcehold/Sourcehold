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

        /**
         * Handles the main menu and preceding intro sequence
         */
        class MainMenu : protected EventConsumer<Mouse>
        {
            public:
                MainMenu(std::shared_ptr<GameManager> man);
                MainMenu(const MainMenu &) = delete;
                ~MainMenu();

                void PlayMusic();
                int Startup();
            protected:
                void onEventReceive(Mouse &event) override;
                int EnterMainMenu();

                double startTime = 0.0, fadeBase = 0.0;
                AudioSource aud_startup, aud_chantloop;

                std::shared_ptr<GameManager> manager;
                std::shared_ptr<TgxFile>  tgx_bg1, tgx_firefly, tgx_taketwo, tgx_present, tgx_logo;
                std::shared_ptr<BinkVideo> intro;

                enum StartupEvent : uint8_t {
                    STARTUP_FIREFLY_LOGO = 0,
                    STARTUP_TAKETWO_LOGO = 1,
                    STARTUP_PRESENT = 2,
                    STARTUP_STRONGHOLD_LOGO = 3,
                    STARTUP_INTRO = 4,
                    STARTUP_MAIN_MENU = 5
                };
                uint8_t current = 0;
        };
    }
}
