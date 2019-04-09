#pragma once

#include <cinttypes>
#include <memory>

#include <SDL2/SDL.h>

#include <Rendering/Rendering.h>
#include <Rendering/Texture.h>
#include <Rendering/Renderer.h>

namespace Sourcehold
{
    namespace Rendering
    {
        using namespace Rendering;

        /*
         * Constructs a tileset from a GM1 container
         */
        class Tileset : public Texture
        {
            std::shared_ptr<Renderer> renderer;
            uint32_t numRows, num;
        public:
            Tileset(std::shared_ptr<Renderer> rend);
            ~Tileset();

            void Allocate(uint32_t num);
            void SetTile(Texture &image, uint32_t index);
            void Clear();

            inline uint32_t GetNumTiles() { return num; }
            SDL_Rect GetTile(uint32_t index);
        protected:
            std::pair<uint32_t, uint32_t> IndexToCoords(uint32_t index);
        };
    }
}
