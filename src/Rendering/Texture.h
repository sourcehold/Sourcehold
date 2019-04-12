#pragma once

#include <cinttypes>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>

#include <Config.h>

#include <Rendering/Renderable.h>

namespace Sourcehold
{
    namespace Rendering
    {
        /**
         * Texture wrapper class, able to create and modify
         * textures, external modification is possible
         * by locking/unlocking the texture stream and obtaining
         * a pointer to the raw pixel data
         */
        class Renderer;
        class Surface;
        class Texture : public Renderable
        {
            std::shared_ptr<Renderer> renderer;
            SDL_Texture *texture = nullptr;
            int width, height, pitch = 0;
            double angle;
            bool locked = false, target = false, valid = false;
            Uint32* pixels = nullptr;
            SDL_RendererFlip flip;
            SDL_TextureAccess access;
        public:
            Texture(std::shared_ptr<Renderer> rend);
            Texture(const Texture &tex);
            ~Texture();

            /**
             * Allocate a new texture. This will turn the class into
             * a streaming texture.
             * Use LockTexture / UnlockTexture to access the pixel data
             */
            bool AllocNewStreaming(int width, int height, int format = SDL_PIXELFORMAT_RGBA8888);

            /**
             * Allocate a new texture using an existing surface. The texture
             * is static and has no read/write support
             */
            bool AllocFromSurface(Surface &surface);

            /**
             * Allocate a new texture in target mode. The pixel
             * data is manipulated via SDL functions, after calling
             * SetTarget. Reset the render target by calling ResetTarget
             * afterwards.
             */
            bool AllocNewTarget(int width, int height, int format = SDL_PIXELFORMAT_RGBA8888);

            void UpdateTexture();
            void Destroy();

            /* Texture manipulation */
            void LockTexture();
            void UnlockTexture();
            void SetTarget();
            void ResetTarget();
            void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
            void Rotate(double angle);
            void FlipHorizontal();
            void FlipVertical();
            void FlipNone();
            void SetAlphaMod(Uint8 alpha);
            void Copy(Texture &other, uint32_t x, uint32_t y, SDL_Rect *rect = nullptr);

            uint32_t *GetData();
            inline bool IsLocked() { return locked; }
            inline SDL_Texture *GetTexture() { return texture; }
            inline int GetWidth() { return width; }
            inline int GetHeight() { return height; }
            inline double GetAngle() { return angle; }
            inline SDL_RendererFlip GetFlip() { return flip; }
        protected:
            Uint32 ToPixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
            void FromPixel(Uint32 value, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);
        };
    }
}
