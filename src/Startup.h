#pragma once

#include <cinttypes>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <memory>

#include "GameManager.h"

#include "GUI/UIState.h"
#include "GUI/MenuUtils.h"

#include "Audio/Song.h"

#include "Events/Event.h"
#include "Events/Mouse.h"

#include "Parsers/TgxFile.h"
#include "Parsers/Gm1File.h"
#include "Parsers/AniFile.h"

#include "Rendering/Texture.h"
#include "Rendering/TextureAtlas.h"
#include "Rendering/BinkVideo.h"

namespace Sourcehold {
    namespace Game
    {
        using namespace Events;
        using namespace Parsers;
        using namespace Rendering;
        using namespace GUI;
        using namespace Audio;

        /**
         * Handles non game states - menus and intro sequence
         */
        class Startup : protected EventConsumer<Mouse> {
        public:
            Startup();
            Startup(const Startup &) = delete;
            ~Startup();

            void PlayMusic();
            UIState Begin();
        protected:
            void onEventReceive(Mouse &event) override;

            double startTime = 0.0, fadeBase = 0.0;
            Song aud_startup;

            /* Resources */
            std::shared_ptr<TgxFile> tgx_firefly, tgx_taketwo, tgx_present, tgx_logo, tgx_firefly_front;
            std::shared_ptr<BinkVideo> intro;

            enum StartupSequence : uint8_t {
                STARTUP_FIREFLY_LOGO = 0,
                STARTUP_TAKETWO_LOGO,
                STARTUP_PRESENT,
                STARTUP_STRONGHOLD_LOGO,
                STARTUP_MULTIPLAYER_INFO,
                STARTUP_INTRO
            };

            UIState currentUIState = UIState::INTRO_SEQUENCE;
            uint8_t currentStartupState = 0;
        };
    }
}
