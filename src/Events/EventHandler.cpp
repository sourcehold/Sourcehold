#include <Events/EventHandler.h>

using namespace Sourcehold::Events;

bool EventHandler::FetchEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        /* Treat SDL_QUIT as a special case */
        if(event.type == SDL_QUIT) {
            return false;
        }

        for(auto const& fn : callbacks) {
            fn.second(event);
        }
    }

    return true;
}

int EventHandler::AddListener(std::function<void(SDL_Event&)> fn) {
    callbacks[callbacks.size()] = fn;
    return callbacks.size();
}

void EventHandler::RemoveListener(int id) {
    callbacks.erase(id);
}