#pragma once

#include <cinttypes>
#include <memory>
#include <utility>
#include <vector>

#include <SDL.h>

#include "Rendering/Texture.h"
#include "Rendering/Surface.h"

namespace Sourcehold {
    namespace Rendering {
        /**
         * Merge multiple textures. First, give it a list of image dimensions
         * to handle, then use the offsets returned by Get() to write to the
         * intermediate surface. When done, call Create() to convert the surface
         * to a static texture.
         */
        class TextureAtlas : public Texture {
            uint32_t num_;
            std::vector<SDL_Rect> entries_;
            Surface surf_;
        public:
            TextureAtlas() = default;
            TextureAtlas(const TextureAtlas &atlas);
            ~TextureAtlas();

            void Allocate(std::vector<std::pair<uint32_t, uint32_t>>& entries);
            void Create();
            SDL_Rect Get(uint32_t index);
            void Clear();
            void Lock();
            void Unlock();
            Uint32 *GetData();

            inline uint32_t GetNumTextures() {
                return num_;
            }
            inline Surface& GetSurface() {
                return surf_;
            }
        protected:
            const uint32_t max_x_resolution_ = 2048;
            std::pair<uint32_t, uint32_t> IndexToCoords(uint32_t index);
        };
    }
}
