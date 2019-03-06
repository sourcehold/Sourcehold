#pragma once

#include <cmath>
#include <memory>

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

                void Allocate(uint32_t numRows);
                void SetEntry(Texture &image, uint32_t x, uint32_t y);
            protected:
                std::shared_ptr<Renderer> renderer;
                uint32_t width, height;
        };
    }
}
