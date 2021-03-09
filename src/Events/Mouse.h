#pragma once

#include <SDL.h>

#include "Events/Event.h"

namespace Sourcehold {
    namespace Events {
        /*
         * Mouse event handler
         */
        class Mouse : public Event {
            SDL_Event event_;
        public:
            Mouse();
            Mouse(const Mouse &mouse) = delete;
            ~Mouse();

            /* Check mouse buttons */
            bool LmbDown();
            bool MmbDown();
            bool RmbDown();
            bool LmbUp();
            bool MmbUp();
            bool RmbUp();

            /* Mouse position */
            uint32_t GetPosX();
            uint32_t GetPosY();

            EventType GetType();
            EventType type;

            void eventCallback(SDL_Event &event) override;
        };
    }
}
