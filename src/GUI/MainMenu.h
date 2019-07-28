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

#include <Audio/Song.h>

namespace Sourcehold {
    namespace GUI {
        using namespace Events;
        using namespace Parsers;
        using namespace Rendering;
        using namespace Audio;

        /**
         * Handles the main menu and all submenus
         */
        class MainMenu {
            StrongholdEdition edition;
            Credits cred;
        public:
            MainMenu();
            MainMenu(const MainMenu &) = delete;
            ~MainMenu();

            UIState EnterMenu();
        protected:
            boost::filesystem::path GetGreetingsSound();

            /* TODO: merge into one function */
            void RenderBackToMain();
            void RenderNext();
            void HideAll();

            /* Common resources */
            Song aud_chantloop;
            Texture screen;
            int mx, my;
            int glareTicks, glareCounter=0;
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
                NEXT,
                BUTTON_END
            } selected = BUTTON_END;

            std::shared_ptr<Gm1File> gm1_icons_main;
            std::shared_ptr<TgxFile> tgx_bg_main, tgx_bg_main2;
            std::shared_ptr<Gm1File> gm1_icons_combat;
            std::shared_ptr<TgxFile> tgx_bg_combat, tgx_bg_combat2;
            std::shared_ptr<Gm1File> gm1_icons_economic;
            std::shared_ptr<TgxFile> tgx_bg_economic;
            std::shared_ptr<Gm1File> gm1_icons_builder;
            std::shared_ptr<TgxFile> tgx_bg_builder;

            std::vector<StaticElement> ui_elems;
            std::vector<std::shared_ptr<TextureAtlas>> ui_tex;

            UIState currentState;

            /* SFX */
            Song aud_greetings, aud_exit;
        };
    }
}
