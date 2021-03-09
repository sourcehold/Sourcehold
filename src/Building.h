#pragma once

#include <cinttypes>
#include <vector>
#include <memory>

#include "Rendering/Renderer.h"
#include "Rendering/Tileset.h"

namespace Sourcehold {
    namespace Parsers {
        class Gm1File;
    }
    namespace Game {
        using namespace Parsers;

        class Building {
            std::shared_ptr<Gm1File> gm1;
            std::vector<uint32_t> walkableTiles;
            uint32_t px, py;
           [[maybe_unused]] uint32_t mapW, mapH;
            bool loaded = false, placed = false;
        public:
            Building(uint32_t mw = 160, uint32_t mh = 160);
            Building(std::weak_ptr<Gm1File> file, uint32_t x = 0, uint32_t y = 0, uint32_t mw = 160, uint32_t mh = 160);
            ~Building();

            /**
             * Render the building preview before
             * a building is placed
             */
            void Preview(uint32_t x, uint32_t y);
            /**
             * Place the building at a certain
             * point on the map (has to be a valid coord)
             */
            void PlaceAt(uint32_t x, uint32_t y);
            void MoveTo(uint32_t x, uint32_t y);
            void Unload();
            void Render();

            bool CanWalkOn(uint32_t x, uint32_t y);
        protected:
            uint32_t CoordToGlobalIndex(uint32_t x, uint32_t y);
            uint32_t CoordToLocalIndex(uint32_t x, uint32_t y);
        };
    }
}
