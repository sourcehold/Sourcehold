#pragma once

#include <SDL2/SDL.h>

namespace Sourcehold
{
    namespace Rendering
    {
        class Renderable
        {
            SDL_Rect rect;
        public:
            Renderable();
            Renderable(const Renderable&);
            ~Renderable();

            void SetRect(SDL_Rect rect);

            inline SDL_Rect GetRect() { return rect; } /* Best function name ever? Possibly. */
        protected:
        };
    }
}
