#include "Events/Keyboard.h"

using namespace Sourcehold::Events;

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

SDL_Keysym Keyboard::Key()
{
    return event_.key.keysym;
}

EventType Keyboard::GetType()
{
    return type;
}

SDL_Event Keyboard::Get()
{
    return event_;
}

void Keyboard::eventCallback(SDL_Event &event)
{
    this->event_ = event;

    Event::SetHandled(true);
    type = ConvertTypes(event.type);
    if(type == KEYDOWN || type == KEYUP || type == TEXTINPUT) {
        Event::SetHandled(false);
    }
}
