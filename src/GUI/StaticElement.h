#pragma once

#include <cinttypes>
#include <functional>
#include <memory>

#include <SDL.h>

#include <GameManager.h>

#include <Rendering/Renderer.h>
#include <Rendering/Texture.h>

#include <Events/Event.h>
#include <Events/Mouse.h>

namespace Sourcehold {
    namespace Rendering {
        using namespace Events;
        using namespace Game;

        /**
         * Static visual element wrapper, handles positioning, scaling and
         * mouse events
         * Use this for UI elements.
         */
        class StaticElement : public EventConsumer<Mouse> {
            Texture *tex = nullptr;
            bool shown = true;
            bool clicked = false;
            bool mouseOver = false;
            bool scaled = false;
            bool transCheck = false;
            double nx, ny, nw, nh, tx, ty, tw, th;
            int mouseX, mouseY, ID;
        public:
            StaticElement(double x = 0.0, double y = 0.0, Texture *t = nullptr);
            StaticElement(const StaticElement &elem);
            ~StaticElement();

            void Hide();
            void Show();
            void Destroy();

            void SetTexture(Texture *t);
            void Translate(int x, int y);
            void Translate(double x, double y);
            void Scale(int w, int h);
            void Scale(double w, double h);

            /**
             * Checks for mouse input only for
             * opaque areas of the image
             */
            void TransparencyCheck(bool check);

            /**
             * Render the element returned by render_fn given the known
             * parameters. This is useful if the renderable may change,
             * E.g. a button is highlighted on mouseover
             */
            void Render(std::function<SDL_Rect()> render_fn);

            bool IsClicked();

            inline bool IsHidden() {
                return !shown;
            }

            inline bool IsMouseOver() {
                return mouseOver;
            }
        protected:
            void onEventReceive(Mouse &event) override;
            /* Fairly complex, so only called once per frame */
            bool IsMouseOverInternal();
        };
    }
}
