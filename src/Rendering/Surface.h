#pragma once

#include <Rendering/Renderer.h>

#include <memory>
#include <SDL2/SDL.h>

namespace Sourcehold
{
    namespace Rendering
    {
        /**
         * An SDL_Surface wrapper class. This is primarily used
         * to create static textures.
         */
        class Surface
        {
            SDL_Surface *surface;
            std::shared_ptr<Renderer> renderer;
            bool locked = false;
            bool valid = false;
            Uint32 *pixels = nullptr;
            uint32_t width, height;
        public:
            Surface(std::shared_ptr<Renderer> rend);
            Surface(const Surface&) = delete;
            ~Surface();

            bool AllocNew(int width, int height, int format = SDL_PIXELFORMAT_RGBA8888);
            void Destroy();
            void LockSurface();
            void UnlockSurface();
            void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
            void Blit(Surface &other, uint32_t x, uint32_t y, SDL_Rect *rect = nullptr);

            Uint32 *GetData();
            inline bool IsValid() { return valid; }
            inline bool IsLocked() { return locked; }
            inline SDL_Surface *GetSurface() { return surface; }
            inline int GetWidth() { return width; }
            inline int GetHeight() { return height; }
        protected:
            Uint32 ToPixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        };
    }
}
