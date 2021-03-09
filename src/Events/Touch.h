#pragma once

#include <SDL.h>
#include <cstdint>

#include "Events/Event.h"
#include "Event.h"

namespace Sourcehold {
    namespace Events {
        class Touch : public Event
        {
            SDL_Event event_;
        public:
            Touch() = default;
            Touch(const Touch&) = delete;

            float GetDx() { return event_.tfinger.dx; }
            float GetDy() { return event_.tfinger.dy; }

            float GetX() { return event_.tfinger.x; }
            float GetY() { return event_.tfinger.y; }

            void eventCallback(SDL_Event &event) override {
                event_ = event;

                type = ConvertTypes(event.type);
                Event::SetHandled(type != FINGERMOTION && type != FINGERUP && type != FINGERDOWN);
            }
        };
    }
}
