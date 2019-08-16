#pragma once

#include <SDL.h>

#include <string>
#include <list>
#include <memory>

#include "Events/EventHandler.h"

namespace Sourcehold {
    namespace Events {
        enum EventType {
            DUMMY,
            KEYBOARD_KEYDOWN,
            KEYBOARD_KEYUP,
            KEYBOARD_TEXTINPUT,
            MOUSE_MOTION,
            MOUSE_BUTTONDOWN,
            MOUSE_BUTTONUP,
            MOUSE_WHEEL
        };

        /*
         * Abstract event class
         */
        class Event {
        public:
            Event() = default;
            Event(const Event &event) = delete;
            ~Event() = default;

            inline void SetHandled(bool h) {
                handled = h;
            }
            inline bool IsHandled() {
                return handled;
            }

            virtual void eventCallback(SDL_Event &event) = 0;
            EventType ConvertTypes(Uint32 sdlcode) {
                switch(sdlcode) {
                case SDL_KEYDOWN:
                    return KEYBOARD_KEYDOWN;
                case SDL_KEYUP:
                    return KEYBOARD_KEYUP;
                case SDL_TEXTINPUT:
                    return KEYBOARD_TEXTINPUT;
                case SDL_MOUSEMOTION:
                    return MOUSE_MOTION;
                case SDL_MOUSEBUTTONDOWN:
                    return MOUSE_BUTTONDOWN;
                case SDL_MOUSEBUTTONUP:
                    return MOUSE_BUTTONUP;
                case SDL_MOUSEWHEEL:
                    return MOUSE_WHEEL;
                default:
                    return DUMMY;
                }
            }
        protected:
            EventType type;
            bool handled = false;
        };

        /*
         * Event consumer class used to dispatch generic events
         * to the front-end (callback driven event stack)
         */
        template<class T>
        class EventConsumer {
            T event;
            int id = 0;
        public:
            ~EventConsumer() {
                RemoveEventListener(id);
            };
            EventConsumer(const EventConsumer &) = delete;
            EventConsumer() : event() {
                std::function<void(SDL_Event&)> fn(std::bind(&EventConsumer::_callback, this, std::placeholders::_1));
                id = AddEventListener(fn);
            }
        protected:
            void _callback(SDL_Event &ev) {
                /* Forward event to implementation */
                event.eventCallback(ev);
                /* If not handled already, hand to consumer */
                if(!event.IsHandled()) onEventReceive(event);
            }
            virtual void onEventReceive(T &event) = 0;
        };
    }
}
