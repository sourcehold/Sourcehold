#pragma once

#include <list>

#include <SDL.h>

#include <Events/Event.h>

namespace Sourcehold {
    namespace Events {
        /*
         * Keyboard event handler
         */
        class Keyboard : public Event {
            SDL_Event event;
            EventType type;
        public:
            Keyboard();
            Keyboard(const Keyboard &key) = delete;
            ~Keyboard();

            SDL_Keysym Key();
            EventType GetType();

            void eventCallback(SDL_Event &event) override;
        protected:
        };
    }
}
