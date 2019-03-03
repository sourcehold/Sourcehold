#pragma once

#include <memory>

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
                Keyboard(std::shared_ptr<SDL_Event> event);
                Keyboard(const Keyboard &key) = delete;
                ~Keyboard();

                void Dispatch() override;
        };
    }
}
