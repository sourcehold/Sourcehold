#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <memory>

namespace Sourcehold
{
    namespace Events
    {
        class Event
        {
            public:
                Event(std::shared_ptr<SDL_Event> event);
                Event(const Event &event) = delete;
                virtual ~Event() = 0;

                virtual void Dispatch() = 0;

                inline std::string GetName() { return name; }
                inline void SetName(const std::string &newName) { name = newName; }

                inline void SetHandled(bool h) { handled = h; }
                inline bool IsHandled() { return handled; }
            protected:
                std::string name;
                bool handled = false;
        };
    }
}
