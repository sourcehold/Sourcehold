#include <random>

#include <GameMap.h>

#include <Parsers/Gm1File.h>

#include <Rendering/TextureAtlas.h>
#include <Rendering/Tileset.h>

using namespace Sourcehold::Game;

#define DIM 160

GameMap::GameMap(std::shared_ptr<GameManager> man) :
    manager(man)
{
    gm1_maypole = man->GetGm1(man->GetDirectory() + "gm/anim_maypole.gm1").lock();
    gm1_tile = man->GetGm1(man->GetDirectory() + "gm/tile_land8.gm1").lock();
    gm1_churches = man->GetGm1(man->GetDirectory() + "gm/tile_churches.gm1").lock();
    gm1_anim = man->GetGm1(man->GetDirectory() + "gm/body_archer.gm1").lock();

    tileset = gm1_tile->GetTileset().lock();
    tiles.resize(DIM * DIM);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 8);

    for(uint32_t i = 0; i < tiles.size(); i++) {
        int tile = dist(rng);
        tiles[i] = tileset->GetTile(tile);
    }

    maypole = manager->AddSlot({ 0, 31 });
}

void GameMap::Render() {
    mult = manager->GetZoomLevel() == Camera::ZOOM_NEAR ? 2 : 1;

    for(uint32_t i = 0; i < tiles.size(); i++) {
        SDL_Rect clip = tiles[i];

        int iy = i / DIM;
        int ix = i % DIM;

        int y = (8 * iy);
        int x = (30 * ix) + (iy % 2 == 0 ? 15 : 0);

        manager->Render(
            *tileset,
            mult * x - manager->CamX(), mult * y - manager->CamY(),
            mult * 30, mult * 16,
            &clip
        );
    }

    Texture &pole = *gm1_maypole->GetTextureAtlas().lock();
    SDL_Rect rect = gm1_maypole->GetTextureAtlas().lock()->Get(manager->GetFrame(maypole));
    manager->Render(pole, mult * rect.w - manager->CamX(), mult * rect.h - manager->CamY(), mult * rect.w, mult * rect.h, &rect);

    Texture &anim = *gm1_anim->GetTextureAtlas().lock();
    rect = gm1_anim->GetTextureAtlas().lock()->Get(10);
    manager->Render(anim, mult * 240 - manager->CamX(), mult * 240 - manager->CamY(), mult * rect.w, mult * rect.h, &rect);

    rect = gm1_anim->GetTextureAtlas().lock()->Get(12);
    manager->Render(anim, mult * 280 - manager->CamX(), mult * 260 - manager->CamY(), mult * rect.w, mult * rect.h, &rect);

    rect = gm1_anim->GetTextureAtlas().lock()->Get(13);
    manager->Render(anim, mult * 250 - manager->CamX(), mult * 275 - manager->CamY(), mult * rect.w, mult * rect.h, &rect);

    Texture &church = *gm1_churches->GetTextureAtlas().lock();
    rect = gm1_churches->GetTextureAtlas().lock()->Get(0);
    manager->Render(church, mult * 550 - manager->CamX(), mult * 90 - manager->CamY(), mult * rect.w, mult * rect.h, &rect);
}
