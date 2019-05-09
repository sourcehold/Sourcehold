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

#include <Menus/MainMenu.h>
#include <Menus/CombatMenu.h>
#include <Menus/BuilderMenu.h>
#include <Menus/EconomicsMenu.h>

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
         * Handles non game states - menus and intro sequence
         */
        class GUI : protected EventConsumer<Mouse>
        {
        public:
            GUI(std::shared_ptr<GameManager> man);
            GUI(const GUI &) = delete;
            ~GUI();

            void PlayMusic();
            int Startup();

        protected:
            void onEventReceive(Mouse &event) override;

            double startTime = 0.0, fadeBase = 0.0;
            AudioSource aud_startup, aud_chantloop;

            /* Resources */
            std::shared_ptr<GameManager> manager;
            std::shared_ptr<TgxFile> tgx_firefly, tgx_taketwo, tgx_present, tgx_logo, tgx_firefly_front;
            std::shared_ptr<BinkVideo> intro;

            enum StartupSequence : uint8_t {
                STARTUP_FIREFLY_LOGO = 0,
                STARTUP_TAKETWO_LOGO = 1,
                STARTUP_PRESENT = 2,
                STARTUP_STRONGHOLD_LOGO = 3,
                STARTUP_INTRO = 4
            };

            UIState currentUIState = UIState::INTRO_SEQUENCE;
            uint8_t currentStartupState = 0;

        private:
            MainMenu mainMenu;
            CombatMenu combatMenu;
            BuilderMenu builderMenu;
            EconomicsMenu economicsMenu;
        };
    }
}
