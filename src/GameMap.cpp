#include <random>

#include "GameMap.h"
#include "GameManager.h"

#include "Parsers/Gm1File.h"
#include "Parsers/TgxFile.h"

#include "Rendering/TextureAtlas.h"
#include "Rendering/Tileset.h"
#include "Rendering/Renderer.h"
#include "Rendering/Camera.h"

using namespace Sourcehold::Game;

GameMap::GameMap(MapDimension type)
{
    static const int dimensions[] = { 160, 200, 300, 400 };
    dim = dimensions[type];
}

GameMap::GameMap(ghc::filesystem::path path)
{
    // TODO //
    LoadFromDisk(path);
    dim = 160;
}

GameMap::~GameMap()
{
}

void GameMap::LoadFromDisk(ghc::filesystem::path path)
{
    MapFile::LoadFromDisk(path);

    gm1_tile = GetGm1("gm/tile_land8.gm1");

    tileset = gm1_tile->GetTileset();
    tiles.resize(static_cast<size_t>(dim * dim));

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 8);

    for (uint32_t i = 0; i < tiles.size(); i++) {
        int tile = static_cast<int>(dist(rng));
        tiles[i] = tileset->GetTile(static_cast<uint32_t>(tile));
    }
}

void GameMap::Render()
{
    Camera& cam = Camera::instance();

    mult = cam.zoom_level_ == ZOOM_NEAR ? 2 : 1;

    for(size_t i = 0; i < tiles.size(); i++) {
        SDL_Rect clip = tiles[i];

        auto iy = static_cast<int>(i) / dim;
        auto ix = static_cast<int>(i) % dim;

        auto y = (8 * iy);
        auto x = (30 * ix) + (iy % 2 == 0 ? 15 : 0);

        Rendering::Render(
            *tileset,
            static_cast<int>(mult) * x - static_cast<int>(cam.pos_x_),
            static_cast<int>(mult) * y - static_cast<int>( cam.pos_y_),
            30*mult,
            16*mult,
            &clip
        );
    }
}

