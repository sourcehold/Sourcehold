#pragma once

#include <cinttypes>
#include <functional>
#include <memory>

#include <SDL.h>

#include "GameManager.h"

#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"

#include "Events/Event.h"
#include "Events/Mouse.h"

namespace Sourcehold {
    namespace Rendering {
        using namespace Events;
        using namespace Game;

        /**
         * Static visual element wrapper, handles positioning, scaling and
         * mouse events
         */
        class StaticElement : public EventConsumer<Mouse> {
            Texture* tex = nullptr;
            Rect<int> tp;
            size_t id;
            SDL_Rect inactive, active; // parts of the texture to be rendered
        public:
            StaticElement() = default;
            StaticElement(Rect<int> tp, Texture *t, SDL_Rect& inactive, SDL_Rect& active, size_t id);
            StaticElement(const StaticElement&);
            ~StaticElement() {}

            void Render();

            inline void Transform(Rect<int> p) { tp = p; }
            inline void SetTexture(Texture* t) { tex = t; }
            inline void SetActiveRect(SDL_Rect& r) { active = r; }
            inline void SetInactiveRect(SDL_Rect& r) { inactive = r; }
            inline void SetID(size_t i) { id = i; }
        public:
            /**
             * Can be used to handle dynamic changes.
             * void onEvent(size_t buttonID, Mouse & event);
             */
            std::function<void(size_t,Mouse&)> onEvent;
            bool visible;
        protected:
            void onEventReceive(Mouse &event) override;
            bool DetectMouseOver(int mx, int my);
        };
    }
}
