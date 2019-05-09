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

namespace Sourcehold
{
    namespace Game
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
            void Draw();
            /* Resources */
            std::shared_ptr<GameManager> manager;
            std::shared_ptr<TgxFile> tgx_bg;
            std::shared_ptr<Gm1File> gm1_icons_combat;

            /* User interface */
            StaticElement ui_combat_campaign, ui_combat_siege, ui_combat_invasion, ui_combat_multiplayer, ui_back_to_main;
        };
    }
}
