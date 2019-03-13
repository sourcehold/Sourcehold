#pragma once

#include <list>

#include <SDL2/SDL.h>

#include <Events/Event.h>

namespace Sourcehold
{
    namespace Events
    {
        /*
         * Keyboard event handler
         */
        class Keyboard : public Event
        {
                std::list<EventType> types;
                SDL_Event event;
                EventType type;
            public:
                Keyboard(std::list<EventType> types);
                Keyboard(const Keyboard &key) = delete;
                ~Keyboard();

                SDL_Keysym Key();
                EventType GetType();
            
                void eventCallback(SDL_Event &event) override;
            protected:
        };
    }
}
