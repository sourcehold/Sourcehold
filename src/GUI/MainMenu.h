#pragma once

#include <cinttypes>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <memory>

#include <GameManager.h>
#include <StaticElement.h>

#include <GUI/UIState.h>
#include <GUI/MenuUtils.h>

#include <Parsers/TgxFile.h>
#include <Parsers/Gm1File.h>

#include <Rendering/Texture.h>

#include <Audio/AudioSource.h>

namespace Sourcehold
{
    namespace GUI
    {
        using namespace Events;
        using namespace Parsers;
        using namespace Rendering;
		using namespace Audio;

        /**
         * Handles the main menu and all submenus
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
            void RenderMain();
            void RenderCombat();
            void RenderEconomic();
            void RenderBuilder();

            /* Common resources */
            Texture screen;
            SDL_Rect border_rect;
            int mx, my;
            int glareCounter;
            std::shared_ptr<GameManager> manager;
            std::shared_ptr<TgxFile> tgx_border;
            std::shared_ptr<Gm1File> gm1_icons_additional;
            StaticElement ui_back_to_main;
            /* Main */
            std::shared_ptr<Gm1File> gm1_icons_main;
            std::shared_ptr<TgxFile> tgx_bg_main;
            StaticElement ui_exit, ui_firefly, ui_tutorial, ui_combat, ui_economic, ui_builder, ui_load, ui_settings;
            /* Combat */
            std::shared_ptr<Gm1File> gm1_icons_combat;
            std::shared_ptr<TgxFile> tgx_bg_combat;
            StaticElement ui_combat_campaign, ui_combat_siege, ui_combat_invasion, ui_combat_multiplayer;
            /* Economic */
            std::shared_ptr<Gm1File> gm1_icons_economic;
            std::shared_ptr<TgxFile> tgx_bg_economic;
            StaticElement ui_economic_campaign, ui_economic_mission, ui_economic_free_build;
            /* Builder */
            std::shared_ptr<Gm1File> gm1_icons_builder;
            std::shared_ptr<TgxFile> tgx_bg_builder;
            StaticElement ui_builder_working_map, ui_builder_stand_alone_mission, ui_builder_siege_that, ui_builder_multiplayer_map;

            /* User interface */
			enum MenuButton : uint8_t {
                NONE_SELECTED,
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
                /* Economic */
                ECO_CAMPAIGN,
                ECO_MISSION,
                ECO_FREEBUILD,
                ECO_BACK_TO_MAIN,
                /* Builder */
                BUILDER_BACK_TO_MAIN,
                BUILDER_WORKING_MAP,
                BUILDER_STANDALONE,
                BUILDER_SIEGE,
                BUILDER_MULTIPLAYER
			} selected = NONE_SELECTED;

			/* SFX */
			AudioSource aud_greetings, aud_exit;
		};
    }
}
