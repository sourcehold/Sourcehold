#pragma once

#include <boost/filesystem.hpp>

#include <cinttypes>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <memory>

#include <GameManager.h>

#include <GUI/StaticElement.h>
#include <GUI/UIState.h>
#include <GUI/Credits.h>
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
            Credits cred;
        public:
            MainMenu();
            MainMenu(const MainMenu &) = delete;
            ~MainMenu();

            UIState EnterMenu();
        protected:
            boost::filesystem::path GetGreetingsSound();

            void RenderMain();
            void RenderCombat();
            void RenderEconomic();
            void RenderBuilder();
            void RenderBackToMain();
            void HideAll();

            /* Common resources */
            AudioSource aud_chantloop;
            Texture screen;
            SDL_Rect border_rect;
            int mx, my;
            int glareCounter;
            std::shared_ptr<TgxFile> tgx_border;
            std::shared_ptr<Gm1File> gm1_icons_additional;

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
                /* Economic */
                ECO_CAMPAIGN,
                ECO_MISSION,
                ECO_FREEBUILD,
                /* Builder */
                BUILDER_WORKING_MAP,
                BUILDER_STANDALONE,
                BUILDER_SIEGE,
                BUILDER_MULTIPLAYER,
                BACK_TO_MAIN,
                BUTTON_END
            } selected = BUTTON_END;

            std::shared_ptr<Gm1File> gm1_icons_main;
            std::shared_ptr<TgxFile> tgx_bg_main;
            std::shared_ptr<Gm1File> gm1_icons_combat;
            std::shared_ptr<TgxFile> tgx_bg_combat;
            std::shared_ptr<Gm1File> gm1_icons_economic;
            std::shared_ptr<TgxFile> tgx_bg_economic;
            std::shared_ptr<Gm1File> gm1_icons_builder;
            std::shared_ptr<TgxFile> tgx_bg_builder;
            std::vector<StaticElement> ui_elems;

            UIState currentState;

			/* SFX */
			AudioSource aud_greetings, aud_exit;
		};
    }
}
