#pragma once

#include <cinttypes>
#include <memory>

#include <Rendering/Renderer.h>
#include <Rendering/TextureAtlas.h>
#include <Rendering/Tileset.h>

#include <Parsers/TgxFile.h>
#include <Parsers/Gm1File.h>

namespace Sourcehold
{
    namespace Game
    {
        using namespace Rendering;
        using namespace Parsers;

        class GameMap
        {
            public:
                GameMap(std::shared_ptr<Renderer> rend);
                GameMap(const GameMap&) = delete;
                ~GameMap() = default;

                void Render();
            protected:
                Gm1File gm1_tile;
                Tileset tileset;
                std::shared_ptr<Renderer> renderer;
        };
    }
}
