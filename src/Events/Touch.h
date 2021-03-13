#pragma once

#include <SDL.h>
#include <cstdint>

#include "Events/Event.h"
#include "Event.h"

namespace Sourcehold {
namespace Events {
class Touch : public Event {
  SDL_Event event;

 public:
  Touch() = default;
  Touch(const Touch &) = delete;

  float GetDx() { return event.tfinger.dx; }
  float GetDy() { return event.tfinger.dy; }

  float GetX() { return event.tfinger.x; }
  float GetY() { return event.tfinger.y; }

  void eventCallback(SDL_Event &event) override {
    this->event = event;

    type = ConvertTypes(event.type);
    Event::SetHandled(type != FINGERMOTION && type != FINGERUP &&
                      type != FINGERDOWN);
  }
};
}  // namespace Events
}  // namespace Sourcehold
