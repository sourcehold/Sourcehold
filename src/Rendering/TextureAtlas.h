#pragma once

#include <cinttypes>
#include <vector>

#include <SDL2/SDL.h>

#include <Rendering/Texture.h>

namespace Sourcehold
{
    namespace Rendering
    {
        class TextureAtlas : protected std::vector<Rendering::Texture>
        {
                SDL_Renderer *renderer;
            public:
                TextureAtlas(SDL_Renderer *rend, uint32_t hint = 4);
                TextureAtlas(const TextureAtlas &atlas);
                ~TextureAtlas();

                void Add(Texture &tex);
                void Set(Texture &tex, uint32_t index);
                Texture &Get(uint32_t index);
                void Remove(uint32_t index);
                void Resize(uint32_t num);
                void Clear();
        };
    }
}
