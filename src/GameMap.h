#pragma once

#include <cinttypes>
#include <memory>
#include <vector>
#include <map>

#include <Rendering/Tileset.h>
#include <Rendering/Renderer.h>

#include <Assets.h>

namespace Sourcehold {
    namespace Parsers {
        class Gm1File;
        class TgxFile;
    }
    namespace Game {
        struct WorldInformation {
            enum Dimensions : uint8_t {
                WORLD_160,
                WORLD_200,
                WORLD_300,
                WORLD_400
            } type;
            std::map<uint16_t, bool> unlockedAssets;
        };

        struct MapTile {
            uint16_t tileset;
        };

        using namespace Rendering;
        using namespace Parsers;

        class GameMap {
            int mult = 1;
            uint16_t maypole;
            std::shared_ptr<Gm1File> gm1_tile, gm1_maypole, gm1_churches, gm1_anim;
            std::shared_ptr<Tileset> tileset;
            std::vector<SDL_Rect> tiles;
        public:
            GameMap();
            GameMap(const GameMap&) = delete;
            ~GameMap() = default;

            void Render();
        protected:
            WorldInformation _worldInfo;
        };
    }
}
