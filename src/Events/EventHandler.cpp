#include <SDL.h>
#include <map>

#include "Events/EventHandler.h"

using namespace Sourcehold;

static std::map<int, std::function<void(SDL_Event&)>> _callbacks;

bool Events::FetchEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        /* Treat SDL_QUIT as a special case */
        if(event.type == SDL_QUIT) {
            return false;
        }

        for(auto const& fn : _callbacks) {
            fn.second(event);
        }
    }

    return true;
}

int Events::AddEventListener(std::function<void(SDL_Event&)> fn)
{
    _callbacks[_callbacks.size()] = fn;
    return _callbacks.size();
}

void Events::RemoveEventListener(int id)
{
    _callbacks.erase(id);
}
