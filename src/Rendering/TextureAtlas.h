#pragma once

#include <cinttypes>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>

#include <Rendering/Texture.h>

namespace Sourcehold
{
    namespace Rendering
    {
        /* TODO
         * Contains a single texture atlas image and is able
         * to expose separate image parts to the renderer
         */
        class TextureAtlas : protected std::vector<Texture>
        {
                std::shared_ptr<Renderer> renderer;
            public:
                TextureAtlas(std::shared_ptr<Renderer> rend, uint32_t hint = 4);
                TextureAtlas(const TextureAtlas &atlas);
                ~TextureAtlas();

                void Add(Texture &tex);
                void Set(Texture &tex, uint32_t index);
                Texture &Get(uint32_t index);
                void Remove(uint32_t index);
                void Resize(uint32_t num);
                void Clear();

                inline uint32_t GetSize() { return vector::size(); }
        };
    }
}
