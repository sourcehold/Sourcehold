#pragma once

#include <memory>

#include "GameMap.h"

#include "GUI/StaticElement.h"
#include "GUI/UIState.h"
#include "GUI/MenuUtils.h"

#include "Rendering/Texture.h"

#include "Events/Keyboard.h"
#include "Events/Mouse.h"

namespace Sourcehold {
    namespace Parsers {
        class TgxFile;
        class Gm1File;
    }
    namespace Game {
        struct Scroll {
            bool shouldScroll = false;
            bool setByKeyboard = false;
            bool setByMouse = false;

            explicit operator bool() {
                return shouldScroll;
            }
        };

        struct ScrollInformation {
            Scroll up;
            Scroll down;
            Scroll right;
            Scroll left;
        };

        using namespace GUI;
        using namespace Parsers;

        /**
         * Handles everything related to the game world, including
         * loading, rendering and moving the camera
         */
        class Building;
        class Entity;
        class World : public GameMap, protected EventConsumer<Keyboard>, protected EventConsumer<Mouse> {
            std::shared_ptr<TgxFile> tgx_right, tgx_bar_bg;
            std::shared_ptr<Gm1File> gm1_scribe, gm1_face, gm1_icons, gm1_floats;
            ScrollInformation scroll;
            /* Menu bar */
            bool menubarShown = true;
            Texture menubar;
            int menuX, menuY, menuW, menuH;
            enum MenuPage : uint8_t {
                MENU_CASTLE = 0,
                MENU_INDUSTRY = 1,
                MENU_FARM = 2,
                MENU_TOWN = 3,
                MENU_WEAPONS = 4,
                MENU_FOOD_PROCESSING = 5
            } currentTab = MENU_CASTLE;
            StaticElement ui_disk, ui_info, ui_delete, ui_revert, ui_tabs[6];
            /* Quick menu */
            StaticElement ui_compass, ui_magnify, ui_lower, ui_hide;
            bool rmbHolding = false;
            Uint32 mouseX, mouseY;
            DialogWindow escMenu;
        public:
            World();
            World(const World&) = delete;
            ~World();

            UIState Play();
        protected:
            void RenderQuickMenu();
            void RenderMenubar();
            void UpdateMenubar();
            void UpdateCamera();
            void LoadMenuAssets();
            void onEventReceive(Keyboard &keyEvent) override;
            void onEventReceive(Mouse &mouseEvent) override;
        };
    }
}
