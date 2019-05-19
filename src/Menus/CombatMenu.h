#pragma once

#include <cinttypes>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <memory>

#include <GameManager.h>
#include <StaticElement.h>
#include <UIState.h>

#include <Parsers/TgxFile.h>
#include <Parsers/Gm1File.h>

#include <Rendering/Texture.h>
#include <Rendering/TextureAtlas.h>

#include <Menus/MenuUtils.h>

namespace Sourcehold
{
    namespace Menus
    {
        using namespace Events;
        using namespace Parsers;
        using namespace Rendering;

        /**
         * Handles the Combat menu
         */
        class CombatMenu
        {
        public:
            CombatMenu(std::shared_ptr<GameManager> man);
            CombatMenu(const CombatMenu &) = delete;
            ~CombatMenu();

            UIState EnterMenu();
        protected:
            void Draw(const int32_t glareCounter);
            /* Resources */
            std::shared_ptr<GameManager> manager;
            std::shared_ptr<TgxFile> tgx_bg;
            std::shared_ptr<Gm1File> gm1_icons_combat;

            /* User interface */
			enum CombatButton {
				COMBAT_NONE,
				COMBAT_CAMPAIGN,
				COMBAT_SIEGE,
				COMBAT_INVASION,
				COMBAT_MULTIPLAYER,
				COMBAT_BACK_TO_MAIN
			} selected = COMBAT_NONE;
            StaticElement ui_combat_campaign, ui_combat_siege, ui_combat_invasion, ui_combat_multiplayer, ui_back_to_main;
        };
    }
}
