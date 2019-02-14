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
                /* Render a texture (screen coordinates) */
                void RenderTexture(Texture &texture, int x, int y);
                void RenderTextureScale(Texture &texture, int x, int y, int w, int h);
                /* Render a texture (normalized coordinates) */
                void RenderTexture(Texture &texture, double x, double y);
                void RenderTextureScale(Texture &texture, double x, double y, double w, double h);
                /* Render a texture to the whole screen */
                void RenderTextureFullscreen(Texture &texture);

                Context CreateContext();
                uint32_t GetWidth();
                uint32_t GetHeight();

                /* Normalize based on window dimensions */
                double NormalizeX(uint32_t c);
                double NormalizeY(uint32_t c);
                uint32_t ToCoordX(double c);
                uint32_t ToCoordY(double c);
            protected:
                SDL_Window *window;
                SDL_Renderer *renderer;
        };
    }
}