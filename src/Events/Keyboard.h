#pragma once

#include <list>

#include <SDL.h>

#include "Events/Event.h"

namespace Sourcehold {
    namespace Events {
        class Keyboard : public Event {
            SDL_Event event;
        public:
            Keyboard();
            Keyboard(const Keyboard &key) = delete;
            ~Keyboard();

            SDL_Keysym Key();
            EventType GetType();
            SDL_Event Get();
            EventType type;

            void eventCallback(SDL_Event &event) override;
        };
    }
}
