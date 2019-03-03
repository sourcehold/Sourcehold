#include <Events/Keyboard.h>

using namespace Sourcehold::Events;

Keyboard::Keyboard(std::list<EventType> types)
{
    this->types = types;
}

Keyboard::~Keyboard() {

}

SDL_Keysym Keyboard::Key() {
    return event.key.keysym;
}

void Keyboard::eventCallback(SDL_Event &event) {
    this->event = event;

    Event::SetHandled(true);
    EventType type = ConvertTypes(event.type);
    if(type == KEYBOARD_KEYDOWN || type == KEYBOARD_KEYUP) {
        Event::SetHandled(false);
    }
}
