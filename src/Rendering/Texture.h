#pragma once

#include <cinttypes>
#include <vector>
#include <memory>

#include <Config.h>
#include <System/Logger.h>

#include <Rendering/Renderer.h>
#include <Rendering/Rendering.h>

namespace Sourcehold
{
    namespace Rendering
    {
        class Renderer;
        class Texture
        {
                std::shared_ptr<Renderer> renderer;
                SDL_Texture *texture;
                std::vector<uint32_t> pixels;
                int width, height;
                double angle;
                SDL_RendererFlip flip;
            public:
                Texture(std::shared_ptr<Renderer> rend);
                Texture(const Texture &tex);
                ~Texture();

                bool AllocNew(int width, int height, int format);
                void UpdateTexture();

                /* Texture manipulation */
                void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
                void Rotate(double angle);
                void FlipHorizontal();
                void FlipVertical();
                void FlipNone();

                inline SDL_Texture *GetTexture() { return texture; }
                inline uint32_t *GetData() { return pixels.data(); }
                inline int GetWidth() { return width; }
                inline int GetHeight() { return height; }
                inline double GetAngle() { return angle; }
                inline SDL_RendererFlip GetFlip() { return flip; }
            protected:
                Uint32 ToPixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        };
    }
}
