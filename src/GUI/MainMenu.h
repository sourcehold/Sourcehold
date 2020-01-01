#pragma once

#include "System/filesystem.h"

#include <cinttypes>
#include <algorithm>
#include <cstdlib>
#include <memory>

#include "GameManager.h"

#include "GUI/Widgets/StaticElement.h"
#include "GUI/Widgets/Button.h"
#include "GUI/UIState.h"
#include "GUI/Credits.h"
#include "GUI/MenuUtils.h"

#include "Parsers/TgxFile.h"
#include "Parsers/Gm1File.h"

#include "Rendering/Texture.h"

#include "Audio/Effect.h"

namespace Sourcehold {
    namespace GUI {
        using namespace Events;
        using namespace Parsers;
        using namespace Rendering;
        using namespace Audio;

        enum MenuButton : uint8_t {
            /* Main */
            MAIN_EXIT,
            MAIN_FIREFLY,
            MAIN_TUTORIAL,
            MAIN_COMBAT,
            MAIN_ECONOMIC,
            MAIN_BUILDER,
            MAIN_LOAD,
            MAIN_SETTINGS,
            /* Combat */
            COMBAT_CAMPAIGN,
            COMBAT_SIEGE,
            COMBAT_INVASION,
            COMBAT_MULTIPLAYER,
            COMBAT_BACK_TO_MAIN,
            COMBAT_CAMPAIGN_BACK,
            COMBAT_CAMPAIGN_NEXT,
            /* Economic */
            ECO_CAMPAIGN,
            ECO_MISSION,
            ECO_FREEBUILD,
            ECO_BACK_TO_MAIN,
            ECO_CAMPAIGN_BACK,
            ECO_CAMPAIGN_NEXT,
            /* Builder */
            BUILDER_WORKING_MAP,
            BUILDER_STANDALONE,
            BUILDER_SIEGE,
            BUILDER_MULTIPLAYER,
            BUILDER_BACK_TO_MAIN,
            BUTTON_END
        };

        /**
         * Handles the main menu and all submenus
         */
        class MainMenu {
            StrongholdEdition edition;
            Credits cred;
            Song aud_chantloop;
            Effect aud_greetings, aud_exit;
            UIState currentState;
            std::vector<StaticElement> ui_elems; // buttons
            std::vector<std::shared_ptr<TextureAtlas>> ui_tex; // texture atlases
        public:
            MainMenu();
            MainMenu(const MainMenu &) = delete;
            ~MainMenu();

            UIState EnterMenu();
        protected:
            ghc::filesystem::path GetGreetingsSound();
            void RenderButtons(MenuButton start, MenuButton end);
            void HideButtons();
        private:
            std::shared_ptr<TgxFile> bg_main[2], bg_combat[2], bg_eco[2], bg_builder[2];
        };
    }
}
