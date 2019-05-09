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
         * Handles the main menu
         */
        class MainMenu
        {
        public:
            MainMenu(std::shared_ptr<GameManager> man);
            MainMenu(const MainMenu &) = delete;
            ~MainMenu();

            UIState EnterMenu();
        protected:
            void Draw();
            /* Resources */
            std::shared_ptr<GameManager> manager;
            std::shared_ptr<TgxFile> tgx_bg1, tgx_firefly, tgx_taketwo, tgx_present, tgx_logo, tgx_firefly_front;
            std::shared_ptr<Gm1File> gm1_icons_main, gm1_icons_additional;

            /* User interface */
            StaticElement ui_exit, ui_firefly, ui_tutorial, ui_combat, ui_economic, ui_builder, ui_load, ui_settings;
        };
    }
}
