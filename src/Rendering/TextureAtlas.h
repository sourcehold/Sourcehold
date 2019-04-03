#pragma once

#include <cinttypes>
#include <memory>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>

#include <Rendering/Texture.h>

namespace Sourcehold
{
    namespace Rendering
    {
        /**
         * Merge multiple textures
         * 
         * TODO: Better space usage
         */
        class TextureAtlas : public Texture
        {
            std::shared_ptr<Renderer> renderer;
            uint32_t num;
            std::vector<SDL_Rect> entries;
        public:
            TextureAtlas(std::shared_ptr<Renderer> rend);
            TextureAtlas(const TextureAtlas &atlas);
            ~TextureAtlas();

            void Allocate(std::vector<std::pair<uint32_t, uint32_t>> entries);
            void Allocate(uint32_t num, uint16_t width, uint16_t height);
            void Set(Texture &image, uint32_t index);
            SDL_Rect Get(uint32_t index);
            void Clear();

            inline uint32_t GetNumTextures() { return num; }
        protected:
            std::pair<uint32_t, uint32_t> IndexToCoords(uint32_t index);
        };
    }
}
