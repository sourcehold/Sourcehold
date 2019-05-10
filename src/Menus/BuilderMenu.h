#pragma once

#include <cinttypes>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <memory>

#include <GameManager.h>
#include <StaticElement.h>
#include <UIState.h>

#include <Menus/MenuUtils.h>

#include <Parsers/TgxFile.h>
#include <Parsers/Gm1File.h>

#include <Rendering/Texture.h>
#include <Rendering/TextureAtlas.h>

namespace Sourcehold
{
    namespace Menus
    {
        using namespace Events;
        using namespace Parsers;
        using namespace Rendering;

        /**
         * Handles the builder menu
         */
        class BuilderMenu
        {
        public:
            BuilderMenu(std::shared_ptr<GameManager> man);
            BuilderMenu(const BuilderMenu &) = delete;
            ~BuilderMenu();

            UIState EnterMenu();
        protected:
            void Draw(const int32_t glareCounter);
            /* Resources */
            std::shared_ptr<GameManager> manager;
            std::shared_ptr<TgxFile> tgx_bg;
            std::shared_ptr<Gm1File> gm1_icons_builder;

            /* User interface */
            StaticElement ui_back_to_main, ui_builder_working_map, ui_builder_stand_alone_mission, ui_builder_siege_that, ui_builder_multiplayer_map;
        };
    }
}
