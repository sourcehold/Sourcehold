#pragma once

#include <cinttypes>
#include <memory>
#include <vector>

#include <GameManager.h>

#include <Rendering/Tileset.h>
#include <Rendering/Renderer.h>

namespace Sourcehold
{
    namespace Game
    {
        using namespace Rendering;
        using namespace Parsers;
        using namespace Events;

        struct MapTile {
            uint16_t tileset;
        };

        class GameMap
        {
            int mult = 1;
            uint16_t maypole;
            std::shared_ptr<Gm1File> gm1_tile, gm1_maypole, gm1_churches, gm1_anim;
            std::shared_ptr<GameManager> manager;
            std::shared_ptr<Tileset> tileset;
            std::vector<SDL_Rect> tiles;
        public:
            GameMap(std::shared_ptr<GameManager> man);
            GameMap(const GameMap&) = delete;
            ~GameMap() = default;

            void Render();
        };
    }
}
