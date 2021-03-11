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
            Texture* tex_ = nullptr;
            Rect<int> tp_;
            size_t id_;
            SDL_Rect inactive_, active_; // parts of the texture to be rendered
        public:
            StaticElement() = default;
            StaticElement(Rect<int> tp, Texture *t, SDL_Rect& inactive, SDL_Rect& active, size_t id);
            StaticElement(const StaticElement&);
            ~StaticElement() {}

            void Render();

            inline void Transform(Rect<int> p) { tp_ = p; }
            inline void SetTexture(Texture* t) { tex_ = t; }
            inline void SetActiveRect(SDL_Rect r) { active_ = r; }
            inline void SetInactiveRect(SDL_Rect r) { inactive_ = r; }
            inline void SetID(size_t i) { id_ = i; }
        public:
            /**
             * Can be used to handle dynamic changes.
             * void onEvent(size_t buttonID, Mouse & event);
             */
            std::function<void(size_t,Mouse&)> onEvent_;
            bool visible_;
        protected:
            void onEventReceive(Mouse &event) override;
            bool DetectMouseOver(int mx, int my);
        };
    }
}
