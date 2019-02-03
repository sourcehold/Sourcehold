#pragma once

#include <SDL2/SDL.h>

namespace OpenSH
{
    namespace Rendering
    {
        struct Context {
            SDL_Renderer *renderer;
            SDL_Window *window;
        };
    }
}
