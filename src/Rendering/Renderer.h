#pragma once

#include <cinttypes>
#include <SDL2/SDL.h>

#include <Rendering/Texture.h>
#include <Rendering/Rendering.h>
#include <System/Logger.h>

namespace OpenSH
{
    namespace Rendering
    {
        class Renderer
        {
            public:
                Renderer();
                ~Renderer();

                void Flush();
                void RenderTextureXY(Texture &texture, uint32_t x, uint32_t y);
                void RenderTextureFullscreen(Texture &texture);
                Context CreateContext();
            protected:
                SDL_Window *window;
                SDL_Renderer *renderer;
        };
    }
}