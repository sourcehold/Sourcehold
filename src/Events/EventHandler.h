#pragma once

#include <SDL.h>

#include <functional>
#include <map>

namespace Sourcehold {
    namespace Events {
        /*
         * Handles getting events from the system and
         * dispatching them to all event listeners
         */
        class EventHandler {
        public:
            EventHandler() = default;
            ~EventHandler() = default;
            EventHandler(const EventHandler &) = delete;

            bool FetchEvents();
            int AddListener(std::function<void(SDL_Event&)> fn);
            void RemoveListener(int id);
        protected:
            std::map<int, std::function<void(SDL_Event&)>> callbacks;
        };
    }
}