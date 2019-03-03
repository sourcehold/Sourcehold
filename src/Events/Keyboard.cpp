#include <Events/Keyboard.h>

using namespace Sourcehold::Events;

Keyboard::Keyboard(std::shared_ptr<SDL_Event> event) : Event(event) {

}

Keyboard::~Keyboard() {

}

void Keyboard::Dispatch() {

}
