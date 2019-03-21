#pragma once

#include <cinttypes>
#include <memory>

#include <SDL2/SDL.h>

#include <Rendering/Texture.h>

namespace Sourcehold
{
    namespace Rendering
    {
        class TextureAtlas : public Texture
        {
                std::shared_ptr<Renderer> renderer;
                uint32_t numRows, num, nx, ny;
            public:
                TextureAtlas(std::shared_ptr<Renderer> rend);
                TextureAtlas(const TextureAtlas &atlas);
                ~TextureAtlas();

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
