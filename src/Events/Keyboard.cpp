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
    return event.key.keysym;
}

EventType Keyboard::GetType()
{
    return type;
}

SDL_Event Keyboard::Get()
{
    return event;
}

void Keyboard::eventCallback(SDL_Event &event)
{
    this->event = event;

    Event::SetHandled(true);
    type = ConvertTypes(event.type);
    if(type == KEYDOWN || type == KEYUP || type == TEXTINPUT) {
        Event::SetHandled(false);
    }
}
