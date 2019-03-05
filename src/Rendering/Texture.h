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
        /*
         * Texture wrapper class, able to create and modify
         * streaming textures, external modification is possible
         * by locking/unlocking the texture stream and obtaining
         * a pointer to the raw pixel data
         */
        class Renderer;
        class Texture
        {
                std::shared_ptr<Renderer> renderer;
                SDL_Texture *texture = nullptr;
                int width, height, pitch = 0;
                double angle;
                bool locked = false;
                Uint32* pixels = nullptr;
                SDL_RendererFlip flip;
            public:
                Texture(std::shared_ptr<Renderer> rend);
                Texture(const Texture &tex);
                ~Texture();

                bool AllocNew(int width, int height, int format);
                void UpdateTexture();

                /* Texture manipulation */
                void LockTexture();
                void UnlockTexture();
                void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
                void Rotate(double angle);
                void FlipHorizontal();
                void FlipVertical();
                void FlipNone();
                void SetAlphaMod(Uint8 alpha);
                void Copy(Texture &other, uint32_t x, uint32_t y);

                uint32_t *GetData();
                inline bool IsLocked() { return locked; }
                inline SDL_Texture *GetTexture() { return texture; }
                inline int GetWidth() { return width; }
                inline int GetHeight() { return height; }
                inline double GetAngle() { return angle; }
                inline SDL_RendererFlip GetFlip() { return flip; }
            protected:
                Uint32 ToPixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        };
    }
}
