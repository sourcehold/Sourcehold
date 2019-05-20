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

#include <Audio/AudioSource.h>

#include <Menus/MenuUtils.h>

namespace Sourcehold
{
    namespace Menus
    {
        using namespace Events;
        using namespace Parsers;
        using namespace Rendering;
		using namespace Audio;

        /**
         * Handles the main menu
         */
        class MainMenu
        {
			StrongholdEdition edition;
        public:
            MainMenu(std::shared_ptr<GameManager> man);
            MainMenu(const MainMenu &) = delete;
            ~MainMenu();

            UIState EnterMenu();
        protected:
            void Draw(const int32_t glareCounter);
            /* Resources */
            std::shared_ptr<GameManager> manager;
			Texture screen;
            std::shared_ptr<TgxFile> tgx_bg1, tgx_firefly, tgx_taketwo, tgx_present, tgx_logo, tgx_firefly_front, tgx_border;
            std::shared_ptr<Gm1File> gm1_icons_main, gm1_icons_additional;

            /* User interface */
			enum MenuButton {
				MAIN_NONE,
				MAIN_EXIT,
				MAIN_FIREFLY,
				MAIN_TUTORIAL,
				MAIN_COMBAT,
				MAIN_ECONOMIC,
				MAIN_BUILDER,
				MAIN_LOAD,
				MAIN_SETTINGS
			} selected = MAIN_NONE;
            StaticElement ui_exit, ui_firefly, ui_tutorial, ui_combat, ui_economic, ui_builder, ui_load, ui_settings;

			/* SFX */
			AudioSource aud_greetings, aud_exit;
		};
    }
}
