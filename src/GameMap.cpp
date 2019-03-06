#include <GameMap.h>

using namespace Sourcehold::Game;

GameMap::GameMap(std::shared_ptr<Renderer> rend) :
    renderer(rend),
    gm1_tile(rend),
    tileset(rend)
{
    gm1_tile.LoadFromDisk("data/gm/tile_land3.gm1", false);
//    tileset.Convert(gm1_tile);
}

void GameMap::Render() {
    renderer->Render(gm1_tile.Get(171));
}
