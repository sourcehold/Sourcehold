#pragma once

#include <memory>
#include <vector>
#include <utility>

#include "Parsers/Gm1File.h"
#include "GUI/Widgets/Container.h"

namespace Sourcehold {
    namespace GUI {
        enum class DialogResult {
            IDLE,
            LOAD,
            SAVE,
            BACK,
            QUIT,
            QUIT_MISSION
        };

        enum class DialogType {
            // Original game //
            QUIT,
            LOAD,
            COMBAT_MENU,
            SIEGE_MENU,
            ECO_MENU,
            SETTINGS,
            ESC_MENU,
            // Sourcehold //
            CAMPAIGN_SELECT_MILITARY,
            CAMPAIGN_SELECT_ECO
        };

        enum class Deco {
            LARGE,
            SMALL
        };

        using namespace Parsers;
        class Dialog : public Container
        {
            int nx, ny; // number of 24x24 pixel 'tiles'
            const int MENU_TILE_DIM = 24;
            std::shared_ptr<Gm1File> icons;
        public:
            Dialog(WidgetLayout l, int nx, int ny);
            ~Dialog();

            enum Position {
                CENTRE,
                ORIGIN
            };

            /**
             * Render the dialog window at the specified position
             * and offset.
             */
            DialogResult Update(Dialog::Position pos, int offX=0, int offY=0);
        protected:
            void RenderBorder(int x, int y, int nx, int ny);
            void RenderDeco(Deco type, int x, int y);
            void RenderTextBox(int x, int y, int w, int h, const std::wstring& text, bool deco);
        };
    }
}
