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
            virtual ~Event() = default;

            inline void SetHandled(bool h) {
                handled_ = h;
            }
            inline bool IsHandled() {
                return handled_;
            }

            inline EventType GetType() {
                return type_;
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
            EventType type_;
            bool handled_ = false;
        };

        /*
         * Event consumer class used to dispatch generic events
         * to the front-end (callback driven event stack)
         */
        template<class T>
        class EventConsumer {
            T event_;
            int id_ = 0;
        public:
            virtual ~EventConsumer() {
                RemoveEventListener(id_);
            };
            EventConsumer(const EventConsumer&) = delete;
            EventConsumer() : event_() {
                std::function<void(SDL_Event&)> fn(std::bind(&EventConsumer::_callback, this, std::placeholders::_1));
                id_ = AddEventListener(fn);
            }
        private:
            void _callback(SDL_Event& ev) {
                /* Forward event to implementation */
                event_.eventCallback(ev);
                /* If not handled already, hand to consumer */
                if (!event_.IsHandled()) onEventReceive(event_);
            }
            virtual void onEventReceive(T& event) = 0;
        };
    }
}
