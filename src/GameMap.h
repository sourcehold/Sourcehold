#pragma once

#include <cinttypes>
#include <memory>
#include <vector>
#include <map>

#include "System/filesystem.h"

#include "Rendering/Tileset.h"
#include "Rendering/Renderer.h"

#include "Parsers/MapFile.h"
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

        using namespace Rendering;
        using namespace Parsers;

        class GameMap : public MapFile {
            int mult = 1, dim;
            std::shared_ptr<Gm1File> gm1_tile;
            std::shared_ptr<Tileset> tileset;
            std::vector<SDL_Rect> tiles;
        public:
            GameMap(MapDimension type);
            GameMap(ghc::filesystem::path path);
            GameMap(const GameMap&) = delete;
            ~GameMap();

            void LoadFromDisk(ghc::filesystem::path path);
            void Render();            
        protected:
        };
    }
}
