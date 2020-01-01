#pragma once

#include <SDL.h>

#include <string>
#include <list>
#include <memory>
#include <functional>

#include "Events/EventHandler.h"

namespace Sourcehold {
    namespace Events {
        enum EventType {
            DUMMY,
            KEYDOWN,
            KEYUP,
            TEXTINPUT,
            MOTION,
            BUTTONDOWN,
            BUTTONUP,
            WHEEL,
            FINGERMOTION,
            FINGERDOWN,
            FINGERUP
        };

        /*
         * Abstract event class
         */
        class Event {
        public:
            Event() = default;
            Event(const Event& event) = delete;
            ~Event() = default;

            inline void SetHandled(bool h) {
                handled = h;
            }
            inline bool IsHandled() {
                return handled;
            }

            inline EventType GetType() {
                return type;
            }

            virtual void eventCallback(SDL_Event& event) = 0;
            EventType ConvertTypes(Uint32 sdlcode) {
                switch (sdlcode) {
                case SDL_KEYDOWN:
                    return KEYDOWN;
                case SDL_KEYUP:
                    return KEYUP;
                case SDL_TEXTINPUT:
                    return TEXTINPUT;
                case SDL_MOUSEMOTION:
                    return MOTION;
                case SDL_MOUSEBUTTONDOWN:
                    return BUTTONDOWN;
                case SDL_MOUSEBUTTONUP:
                    return BUTTONUP;
                case SDL_MOUSEWHEEL:
                    return WHEEL;
                case SDL_FINGERMOTION:
                    return FINGERMOTION;
                case SDL_FINGERDOWN:
                    return FINGERDOWN;
                case SDL_FINGERUP:
                    return FINGERUP;
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
            EventConsumer(const EventConsumer&) = delete;
            EventConsumer() : event() {
                std::function<void(SDL_Event&)> fn(std::bind(&EventConsumer::_callback, this, std::placeholders::_1));
                id = AddEventListener(fn);
            }
        private:
            void _callback(SDL_Event& ev) {
                /* Forward event to implementation */
                event.eventCallback(ev);
                /* If not handled already, hand to consumer */
                if (!event.IsHandled()) onEventReceive(event);
            }
            virtual void onEventReceive(T& event) = 0;
        };
    }
}
