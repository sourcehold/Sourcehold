#pragma once

#include <cinttypes>
#include <SDL2/SDL.h>

#include <Rendering/Texture.h>
#include <Rendering/Rendering.h>
#include <System/Logger.h>

namespace Sourcehold
{
    namespace Rendering
    {
        class Renderer
        {
            public:
                Renderer();
                ~Renderer();

                void Flush();
                void RenderTextureXY(Texture &texture, int x, int y);
                void RenderTextureFullscreen(Texture &texture);
                Context CreateContext();
            protected:
                SDL_Window *window;
                SDL_Renderer *renderer;
        };
    }
}