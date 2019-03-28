#pragma once

#include <list>

#include <SDL2/SDL.h>

#include <Events/Event.h>

namespace Sourcehold
{
    namespace Events
    {
        /*
         * Mouse event handler
         */
        class Mouse : public Event
        {
                std::list<EventType> types;
                SDL_Event event;
                EventType type;
            public:
                Mouse(std::list<EventType> types);
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
                void eventCallback(SDL_Event &event) override;
        };
    }
}
