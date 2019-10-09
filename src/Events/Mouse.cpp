#include "Events/Mouse.h"

using namespace Sourcehold::Events;

Mouse::Mouse()
{

}

Mouse::~Mouse()
{

}

bool Mouse::LmbDown()
{
    if(event.button.state == SDL_PRESSED && event.button.button == SDL_BUTTON_LEFT) return true;
    return false;
}

bool Mouse::MmbDown()
{
    if(event.button.state == SDL_PRESSED && event.button.button == SDL_BUTTON_MIDDLE) return true;
    return false;
}

bool Mouse::RmbDown()
{
    if(event.button.state == SDL_PRESSED && event.button.button == SDL_BUTTON_RIGHT) return true;
    return false;
}

bool Mouse::LmbUp()
{
    if(event.button.state == SDL_RELEASED && event.button.button == SDL_BUTTON_LEFT) return true;
    return false;
}

bool Mouse::MmbUp()
{
    if(event.button.state == SDL_RELEASED && event.button.button == SDL_BUTTON_MIDDLE) return true;
    return false;
}

bool Mouse::RmbUp()
{
    if(event.button.state == SDL_RELEASED && event.button.button == SDL_BUTTON_RIGHT) return true;
    return false;
}

uint32_t Mouse::GetPosX()
{
    return event.motion.x;
}

uint32_t Mouse::GetPosY()
{
    return event.motion.y;
}

EventType Mouse::GetType()
{
    return type;
}

void Mouse::eventCallback(SDL_Event &event)
{
    this->event = event;

    Event::SetHandled(true);
    type = ConvertTypes(event.type);
    if(
        type == MOTION ||
        type == BUTTONDOWN ||
        type == BUTTONUP ||
        type == WHEEL
    ) {
        Event::SetHandled(false);
    }
}
