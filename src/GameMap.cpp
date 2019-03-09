#include <GameMap.h>

using namespace Sourcehold::Game;

GameMap::GameMap(std::shared_ptr<GameManager> man) :
    EventConsumer<Keyboard>(man->GetHandler()),
    EventConsumer<Mouse>(man->GetHandler()),
    manager(man),
    gm1_tile(man),
    gm1_scribe(man),
    tileset(man),
    edge_left(man),
    edge_right(man),
    middle(man),
    bar(man)
{
    /* TESTING STUFF */

    gm1_scribe.LoadFromDisk("data/gm/scribe.gm1");
    edge_left.LoadFromDisk("data/gfx/edge1280l.tgx");
    edge_right.LoadFromDisk("data/gfx/edge1280r.tgx");
    middle.LoadFromDisk("data/gfx/1280r.tgx");
//    gm1_tile.LoadFromDisk("data/gm/tile_land8.gm1", false);
//    gm1_tile.ReadTiles(tileset);

    Texture &scribe = gm1_scribe.GetEntry(0).image;

    bar.AllocNew(
        edge_left.GetWidth() + edge_right.GetWidth() + middle.GetWidth(),
        edge_left.GetHeight() + edge_right.GetHeight() + middle.GetHeight()
    );
    bar.LockTexture();
    edge_left.LockTexture();
    edge_right.LockTexture();
    middle.LockTexture();
    scribe.LockTexture();

    bar.Copy(edge_left, 0, bar.GetHeight() - edge_left.GetHeight());
    bar.Copy(middle, edge_left.GetWidth(), bar.GetHeight() - middle.GetHeight());
    bar.Copy(edge_right, edge_left.GetWidth() + middle.GetWidth(), bar.GetHeight() - edge_right.GetHeight());
    bar.Copy(scribe, edge_left.GetWidth() + middle.GetWidth() - scribe.GetWidth(), bar.GetHeight() - scribe.GetHeight());

    bar.UnlockTexture();
    edge_left.UnlockTexture();
    edge_right.UnlockTexture();
    middle.UnlockTexture();
    scribe.UnlockTexture();
}

void GameMap::Render() {
    manager->Render(bar, 0.0, 0.5, 1.0, 0.5);
}

void GameMap::onEventReceive(Keyboard &keyEvent) {
    switch(keyEvent.Key().sym) {
        case SDLK_LEFT: manager->TranslateCam(-2, 0); break;
        case SDLK_RIGHT: manager->TranslateCam(2, 0); break;
        case SDLK_UP: manager->TranslateCam(0, -2); break;
        case SDLK_DOWN: manager->TranslateCam(0, 2); break;
        default: break;
    }
}

void GameMap::onEventReceive(Mouse &mouseEvent) {

}
