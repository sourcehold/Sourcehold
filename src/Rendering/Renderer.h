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
                Renderer(const Renderer &rend);
                ~Renderer();

                void Flush();
                void RenderTexture(Texture &texture, int x, int y);
                void RenderTextureScale(Texture &texture, int x, int y, int w, int h);
                void RenderTextureFullscreen(Texture &texture);
                Context CreateContext();
            protected:
                SDL_Window *window;
                SDL_Renderer *renderer;
        };
    }
}