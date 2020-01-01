#pragma once

#include "GUI/Widgets/StaticElement.h"
#include "GUI/MenuUtils.h"

#include "Events/Event.h"
#include "Events/Keyboard.h"
#include "Events/Mouse.h"

#include "UIState.h"

#include "Parsers/Gm1File.h"
#include "Parsers/TgxFile.h"

#include "Rendering/Texture.h"

namespace Sourcehold {
    namespace GUI {
        using namespace Parsers;
        using namespace Rendering;
        using namespace Events;

        class IngameGUI :
            private EventConsumer<Keyboard>,
            private EventConsumer<Mouse>
        {
            bool rmbHolding = false, menubarShown = true;
            enum MenuPage : uint8_t {
                MENU_CASTLE = 0,
                MENU_INDUSTRY = 1,
                MENU_FARM = 2,
                MENU_TOWN = 3,
                MENU_WEAPONS = 4,
                MENU_FOOD_PROCESSING = 5
            } currentTab = MENU_CASTLE;
            StaticElement ui_disk, ui_info, ui_delete, ui_revert, ui_tabs[6];
            StaticElement ui_compass, ui_magnify, ui_lower, ui_hide;
            std::shared_ptr<TgxFile> tgx_right, tgx_left;
            std::shared_ptr<Gm1File> gm1_scribe, gm1_face, gm1_icons, gm1_floats;
        public:
            IngameGUI();
            ~IngameGUI();

            bool Render();

            int GetMenubarHeight();
        protected:
            void LoadMenuAssets();
            void RenderQuickMenu();
            void RenderMenubar();
        private:
            void onEventReceive(Keyboard& keyEvent) override;
            void onEventReceive(Mouse& mouseEvent) override;
        };
    }
}
