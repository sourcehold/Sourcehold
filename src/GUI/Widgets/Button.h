#pragma once

#include <string>
#include <functional>

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
        class Button : public EventConsumer<Mouse>, public Widget
        {
            DialogButton style;
            std::wstring text;
            bool selected;
            std::function<void(Mouse & ev)> handler;
        public:
            Button(DialogButton style, const std::wstring &text);
            ~Button();
            
            void Update(Rect<int> constraints) override;

            inline void SetOnClick(std::function<void(Mouse & ev)> fn) { handler = fn; }
        protected:
            void onEventReceive(Mouse& event) override;
        };
    }
}
