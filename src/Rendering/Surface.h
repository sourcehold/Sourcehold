#pragma once

#include <memory>
#include <cstring>
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
            Uint32 *pixels = nullptr;
        public:
            Surface();
            Surface(const Surface&);
            ~Surface();

            bool AllocNew(int width, int height);
            void Destroy();
            void LockSurface();
            void UnlockSurface();
            void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
            void Blit(Surface &other, uint32_t x, uint32_t y, SDL_Rect *rect = nullptr);
            void Fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

            Uint32 *GetData();
            inline bool IsValid() {
                return surface;
            }
            inline bool IsLocked() {
                return locked;
            }
            inline SDL_Surface *GetSurface() {
                return surface;
            }
            inline int GetWidth() {
                return surface->w;
            }
            inline int GetHeight() {
                return surface->h;
            }
        protected:
            Uint32 ToPixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        };
    }
}
