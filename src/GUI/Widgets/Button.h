#pragma once

#include <string>

#include "Events/Event.h"
#include "Events/Mouse.h"

#include "GUI/Widgets/Widget.h"

namespace Sourcehold {
    namespace GUI {
        // TODO: names?
        enum DialogButton : uint8_t {
            BUTTON_1 = 0,
            BUTTON_2,
            BUTTON_3,
            BUTTON_4,
            BUTTON_5,
            BUTTON_6,
            BUTTON_7,
            BUTTON_8,
            BUTTON_9
        };

        using namespace Events;
        class Button : protected EventConsumer<Mouse>, public Widget
        {
            DialogButton style;
            std::wstring text;
            bool clicked = false;
        public:
            Button(DialogButton style, const std::wstring &text);
            ~Button();

            void Update(Rect<int> constraints) override;
        protected:
            void onEventReceive(Mouse& event) override;
        };
    }
}
