#pragma once

#include <cinttypes>
#include <memory>

#include <SDL2/SDL.h>

#include <System/Logger.h>

#include <Rendering/Texture.h>
#include <Rendering/Rendering.h>
#include <Rendering/Camera.h>

namespace Sourcehold
{
    namespace Rendering
    {
        /*
         * Handles everything related to rendering
         */
        class Texture;
        class Renderer : public Camera
        {
            public:
                Renderer();
                Renderer(const Renderer &rend);
                ~Renderer();

                /* Render everything */
                void Flush();
                /* Render a texture (screen coordinates) */
                void Render(Texture &texture, int x, int y);
                void Render(Texture &texture, int x, int y, int w, int h);
                /* Render a texture (normalized coordinates) */
                void Render(Texture &texture, double x, double y);
                void Render(Texture &texture, double x, double y, double w, double h);
                /* Render a texture to the whole screen */
                void Render(Texture &texture);

                uint32_t GetWidth();
                uint32_t GetHeight();
                inline SDL_Renderer *GetRenderer() { return renderer; }

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