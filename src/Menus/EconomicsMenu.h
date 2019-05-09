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
         * Handles the Economics menu
         */
        class EconomicsMenu
        {
        public:
            EconomicsMenu(std::shared_ptr<GameManager> man);
            EconomicsMenu(const EconomicsMenu &) = delete;
            ~EconomicsMenu();

            UIState EnterMenu();
        protected:
            void Draw(const int32_t glareCounter);
            /* Resources */
            std::shared_ptr<GameManager> manager;
            std::shared_ptr<TgxFile> tgx_bg;
            std::shared_ptr<Gm1File> gm1_icons_economics;

            /* User interface */
            StaticElement ui_economic_campaign, ui_economic_mission, ui_economic_free_build, ui_back_to_main;
        };
    }
}
