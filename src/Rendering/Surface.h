#pragma once

#include <memory>
#include <SDL.h>

#include "Rendering/Renderer.h"

namespace Sourcehold {
    namespace Rendering {
        /**
         * An SDL_Surface wrapper class. This is primarily used
         * to create static textures.
         */
        class Surface {
            SDL_Surface *surface;
            bool locked = false;
            bool valid = false;
            int format;
            Uint32 *pixels = nullptr;
            uint32_t width, height;
        public:
            Surface();
            Surface(const Surface&);
            ~Surface();

            bool AllocNew(int width, int height, int format = SDL_PIXELFORMAT_RGBA4444);
            void Destroy();
            void LockSurface();
            void UnlockSurface();
            void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
            void Blit(Surface &other, uint32_t x, uint32_t y, SDL_Rect *rect = nullptr);
            void Fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

            Uint32 *GetData();
            inline bool IsValid() {
                return valid;
            }
            inline bool IsLocked() {
                return locked;
            }
            inline SDL_Surface *GetSurface() {
                return surface;
            }
            inline int GetWidth() {
                return width;
            }
            inline int GetHeight() {
                return height;
            }
        protected:
            Uint32 ToPixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        };
    }
}
