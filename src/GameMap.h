#pragma once

#include <cinttypes>
#include <memory>
#include <vector>
#include <map>

#include <boost/filesystem.hpp>

#include "Rendering/Tileset.h"
#include "Rendering/Renderer.h"

#include "Assets.h"

namespace Sourcehold {
    namespace Parsers {
        class Gm1File;
        class TgxFile;
    }
    namespace Game {
        using namespace Assets;

        struct WorldInformation {
            MapDimension type;
            std::map<Building, bool> unlockedBuildings;
        };

        struct MapTile {
            uint8_t tileset;
            uint8_t height;
        };

        using namespace Rendering;
        using namespace Parsers;

        class GameMap {
            int mult = 1;
            std::shared_ptr<Gm1File> gm1_tile;
            std::shared_ptr<Tileset> tileset;
            std::vector<SDL_Rect> tiles;
        public:
            GameMap();
            GameMap(const GameMap&) = delete;
            ~GameMap() = default;

            void LoadFromDisk(boost::filesystem::path path);
            void Render();
        protected:
        };
    }
}
