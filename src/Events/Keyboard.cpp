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
    if(type == KEYBOARD_KEYDOWN || type == KEYBOARD_KEYUP || type == KEYBOARD_TEXTINPUT) {
        Event::SetHandled(false);
    }
}
