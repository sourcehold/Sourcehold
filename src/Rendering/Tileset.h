#pragma once

#include <cmath>
#include <memory>

#include <utility>

#include <SDL2/SDL.h>

#include <System/Logger.h>

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
            public:
                Tileset(std::shared_ptr<Renderer> rend);
                ~Tileset();

                void Allocate(uint32_t num);
                void SetEntry(Texture &image, uint32_t index);
                
                SDL_Rect GetEntry(uint32_t index);
            protected:
                std::pair<uint32_t, uint32_t> IndexToCoords(uint32_t index);

                std::shared_ptr<Renderer> renderer;
                uint32_t width, height;
                uint32_t numRows;
        };
    }
}
