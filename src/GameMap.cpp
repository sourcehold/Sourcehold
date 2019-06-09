#include <random>

#include <GameMap.h>
#include <GameManager.h>

#include <Parsers/Gm1File.h>
#include <Parsers/TgxFile.h>

#include <Rendering/TextureAtlas.h>
#include <Rendering/Tileset.h>
#include <Rendering/Renderer.h>
#include <Rendering/Camera.h>

using namespace Sourcehold::Game;

#define DIM 160

GameMap::GameMap()
{
    gm1_maypole = GetGm1(GetDirectory() / "gm/anim_maypole.gm1").lock();
    gm1_tile = GetGm1(GetDirectory() / "gm/tile_land8.gm1").lock();
    gm1_churches = GetGm1(GetDirectory() / "gm/tile_churches.gm1").lock();
    gm1_anim = GetGm1(GetDirectory() / "gm/body_archer.gm1").lock();

    tileset = gm1_tile->GetTileset().lock();
    tiles.resize(DIM * DIM);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 8);

    for(uint32_t i = 0; i < tiles.size(); i++) {
        int tile = dist(rng);
        tiles[i] = tileset->GetTile(tile);
    }
}

void GameMap::Render() {
    mult = GetZoomLevel() == ZOOM_NEAR ? 2 : 1;

    for(uint32_t i = 0; i < tiles.size(); i++) {
        SDL_Rect clip = tiles[i];

        int iy = i / DIM;
        int ix = i % DIM;

        int y = (8 * iy);
        int x = (30 * ix) + (iy % 2 == 0 ? 15 : 0);

        Rendering::Render(
            *tileset,
            int(mult * x - CamX()), int(mult * y - CamY()),
            &clip
        );
    }

    Texture &pole = *gm1_maypole->GetTextureAtlas().lock();
    SDL_Rect rect = gm1_maypole->GetTextureAtlas().lock()->Get(0);
    Rendering::Render(pole, mult * rect.w - CamX(), mult * rect.h - CamY(), mult * rect.w, mult * rect.h, &rect);

    Texture &anim = *gm1_anim->GetTextureAtlas().lock();
    rect = gm1_anim->GetTextureAtlas().lock()->Get(10);
    Rendering::Render(anim, mult * 240 - CamX(), mult * 240 - CamY(), mult * rect.w, mult * rect.h, &rect);

    rect = gm1_anim->GetTextureAtlas().lock()->Get(12);
    Rendering::Render(anim, mult * 280 - CamX(), mult * 260 - CamY(), mult * rect.w, mult * rect.h, &rect);

    rect = gm1_anim->GetTextureAtlas().lock()->Get(13);
    Rendering::Render(anim, mult * 250 - CamX(), mult * 275 - CamY(), mult * rect.w, mult * rect.h, &rect);

    Texture &church = *gm1_churches->GetTextureAtlas().lock();
    rect = gm1_churches->GetTextureAtlas().lock()->Get(0);
    Rendering::Render(church, mult * 550 - CamX(), mult * 90 - CamY(), mult * rect.w, mult * rect.h, &rect);
}
