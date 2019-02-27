#pragma once

#include <SDL2/SDL.h>

#include <string>

namespace Sourcehold
{
    namespace Events
    {
        class Event
        {
            public:
                Event();
                Event(const std::string &name);
                Event(const Event &event);
                virtual ~Event();

                inline std::string GetName() { return name; }
                inline void SetName(const std::string &newName) { name = newName; }
            protected:
                std::string name;
        };
    }
}
