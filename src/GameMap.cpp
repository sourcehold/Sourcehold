#include <GameMap.h>
#include <Rendering/TextureAtlas.h>
#include <Rendering/Tileset.h>
#include <Rendering/StaticElement.h>

using namespace Sourcehold::Game;

#define DIM 23

GameMap::GameMap(std::shared_ptr<GameManager> man) :
    EventConsumer<Keyboard>(man->GetHandler()),
    EventConsumer<Mouse>(man->GetHandler()),
    manager(man)
{
    gm1_maypole = man->GetGm1(man->GetDirectory() + "gm/anim_maypole.gm1");
    gm1_tile = man->GetGm1(man->GetDirectory() + "gm/tile_land8.gm1");
    gm1_churches = man->GetGm1(man->GetDirectory() + "gm/tile_churches.gm1");

    tiles.resize(DIM * DIM * 2);
    for(uint32_t i = 0; i < tiles.size(); i++) {
        tiles[i] = gm1_tile->GetTile(i % gm1_tile->GetNumTiles());
    }

    maypole = manager->AddSlot({ 0, 31 });
}

void GameMap::Render() {
//    for(uint32_t i = 0; i < tiles.size(); i++) {
//        SDL_Rect clip = tiles[i];
//
//        int iy = i / DIM;
//        int ix = i % DIM;
//
//        int y = (8 * iy);
//        int x = (30 * ix) + (iy % 2 == 0 ? 15 : 0);
//
//        manager->Render(
//            *gm1_tile,
//            mult * x - manager->CamX(), mult * y - manager->CamY(),
//            mult * 30, mult * 16,
//            &clip
//        );
//    }
//
//    Texture & tex = gm1_maypole->Get(manager->GetFrame(maypole));
//    manager->Render(
//        tex,
//        mult * 100 - manager->CamX(), mult * 10 - manager->CamY(),
//        mult * tex.GetWidth(), mult * tex.GetHeight()
//    );
//
//    manager->Render(
//        tex,
//        mult * 1 - manager->CamX(), mult * 1 - manager->CamY(),
//        mult * tex.GetWidth(), mult * tex.GetHeight()
//    );
//
//    manager->Render(
//        tex,
//        mult * 200 - manager->CamX(), mult * 20 - manager->CamY(),
//        mult * tex.GetWidth(), mult * tex.GetHeight()
//    );
}

void GameMap::onEventReceive(Keyboard &keyEvent) {
    if(keyEvent.GetType() == KEYBOARD_KEYDOWN) {
        switch(keyEvent.Key().sym) {
            case SDLK_LEFT: manager->TranslateCam(-6, 0); break;
            case SDLK_RIGHT: manager->TranslateCam(6, 0); break;
            case SDLK_UP: manager->TranslateCam(0, -6); break;
            case SDLK_DOWN: manager->TranslateCam(0, 6); break;
            case SDLK_SPACE: {
                if(mult == 1) mult = 2;
                else mult = 1;
            }break;
            default: break;
        }
    }
}

void GameMap::onEventReceive(Mouse &mouseEvent) {

}
