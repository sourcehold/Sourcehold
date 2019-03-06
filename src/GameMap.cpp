#include <GameMap.h>

using namespace Sourcehold::Game;

GameMap::GameMap(std::shared_ptr<GameManager> man) :
    EventConsumer<Keyboard>(man->GetHandler()),
    EventConsumer<Mouse>(man->GetHandler()),
    manager(man),
    gm1_tile(man),
    tileset(man)
{
    gm1_tile.LoadFromDisk("data/gm/tile_land3.gm1", false);
    gm1_tile.ReadTiles(tileset);
}

void GameMap::Render() {
    manager->Render(tileset);
}

void GameMap::onEventReceive(Keyboard &keyEvent) {

}

void GameMap::onEventReceive(Mouse &mouseEvent) {

}
