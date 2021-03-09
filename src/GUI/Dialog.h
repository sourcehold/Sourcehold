#pragma once

#include <memory>
#include <vector>
#include <utility>

#include "Parsers/Gm1File.h"
#include "GUI/Widgets/Container.h"

namespace Sourcehold {
    namespace GUI {
        enum class Deco {
            LARGE,
            SMALL,
            NONE
        };

        using namespace Parsers;
        class Dialog : public Container
        {
            const int menu_title_dim_ = 24;
            const int margin_x_ = 12;
            const int margin_y_ = 4;
            int nx_, ny_, tw_; // number of 24x24 pixel 'tiles', textbox width in tiles
            bool text_box_, border_;
            Deco deco_;
            std::wstring text_;
            std::shared_ptr<Gm1File> icons_;
        public:
            Dialog(WidgetLayout l, int nx, int ny, const std::wstring& text = L"", bool textbox = false, Deco deco = Deco::LARGE, int textboxW = 0, bool border = true);
            ~Dialog();

            enum Position {
                CENTRE,
                ORIGIN
            };

            /**
             * Render the dialog window at the specified position
             * and offset.
             */
            void Render(Dialog::Position pos, int offX=0, int offY=0);
        public:
            std::function<void()> onExit;
        protected:
            void RenderBorder(int x, int y, int nx, int ny);
            void RenderDeco(Deco type, int x, int y);
            void RenderTextBox(int x, int y, int w, int h, const std::wstring& text, Deco deco);
        };
    }
}
