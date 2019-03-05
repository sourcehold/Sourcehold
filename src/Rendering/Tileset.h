#pragma once

#include <cmath>
#include <memory>

#include <System/Logger.h>

#include <Parsers/Gm1File.h>

#include <Rendering/Rendering.h>
#include <Rendering/Texture.h>
#include <Rendering/Renderer.h>

namespace Sourcehold
{
    namespace Rendering
    {
        using namespace Rendering;
        using namespace Parsers;

        /*
         * Constructs a tileset from a GM1 container
         */
        class Tileset : public Texture
        {
            public:
                Tileset(std::shared_ptr<Renderer> rend);
                ~Tileset();

                bool Convert(Gm1File &file);
            protected:
                std::shared_ptr<Renderer> renderer;
        };
    }
}
