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

namespace Sourcehold
{
    namespace Rendering
    {
        using namespace Events;
        using namespace Game;

        /**
         * Static visual element wrapper, handles positioning, scaling and
         * mouse events
         * Use this for UI elements.
         */
        class StaticElement : public EventConsumer<Mouse>
        {
            std::shared_ptr<GameManager> manager;
            SDL_Rect rect;
            bool shown = false;
            bool clicked = false;
            double nx, ny, nw, nh, tx, ty, tw, th;
            int mouseX, mouseY;
			bool scaled = false;
        public:
            StaticElement(std::shared_ptr<GameManager> mgr, double x = 0.0, double y = 0.0, SDL_Rect r = { 0,0,0,0 });
            StaticElement(const StaticElement &elem);
            ~StaticElement();

            void Hide();
            void Show();
            void Destroy();

            void Translate(int x, int y);
            void Translate(double x, double y);
            void Scale(int w, int h);
            void Scale(double w, double h);

            /**
             * Render the element returned by render_fn given the known
             * parameters. This is useful if the renderable may change,
             * E.g. a button is highlighted on mouseover
             */
            void Render(std::function<Texture&()> render_fn);

            /**
             * Render the element given the known parameters
             */
            void Render(Texture &elem, bool whole = false);
            bool IsMouseOver();
            bool IsClicked();

            inline bool IsHidden() { return !shown; }
        protected:
			void GetRealCoords(int &rx, int &ry, int& rw, int &rh);
            void onEventReceive(Mouse &event) override;
        };
    }
}
