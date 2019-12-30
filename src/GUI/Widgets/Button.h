#pragma once

#include "Events/Mouse.h"
#include "GUI/MenuUtils.h"

namespace Sourcehold {
    namespace GUI {
        class Button : protected EventConsumer<Mouse>
        {
            DialogButton style;
            uint32_t x, y;
            std::wstring text;
            bool clicked = false;
        public:
            Button(DialogButton style, uint32_t x, uint32_t y, const std::wstring &text);
            ~Button();

            /**
             * Renders the button and updates the state.
             * Returns true if the button was pressed
             */
            bool Update();
        protected:
            void onEventReceive(Mouse& event) override;
        };
    }
}
