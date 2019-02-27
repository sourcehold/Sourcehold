#pragma once

#include <SDL2/SDL.h>

#include <Events/Event.h>

namespace Sourcehold
{
    namespace Events
    {
        class Keyboard : public Event
        {
                enum EventType {
                    KEYDOWN,
                    KEYUP
                } type;
            public:
                Keyboard();
                Keyboard(const Keyboard &key);
                ~Keyboard();

                void Generate(SDL_Event &ev);
        };
    }
}
