#pragma once

#include <Events/Event.h>

namespace Sourcehold
{
    namespace Events
    {
        template<class T>
        class EventHandler
        {
            public:
                EventHandler() = default;
                EventHandler(const EventHandler&) = delete;
                ~EventHandler() = default;
            protected:
        };
    }
}
