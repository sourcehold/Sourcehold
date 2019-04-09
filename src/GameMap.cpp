#include <random>

#include <GameMap.h>
#include <Rendering/TextureAtlas.h>
#include <Rendering/Tileset.h>

using namespace Sourcehold::Game;

#define DIM 160

GameMap::GameMap(std::shared_ptr<GameManager> man) :
    EventConsumer<Keyboard>(man->GetHandler()),
    EventConsumer<Mouse>(man->GetHandler()),
    menubar(man),
    manager(man)
{
    gm1_maypole = man->GetGm1(man->GetDirectory() + "gm/anim_maypole.gm1").lock();
    gm1_tile = man->GetGm1(man->GetDirectory() + "gm/tile_land8.gm1").lock();
    gm1_churches = man->GetGm1(man->GetDirectory() + "gm/tile_churches.gm1").lock();
    gm1_anim = man->GetGm1(man->GetDirectory() + "gm/body_archer.gm1").lock();

    /* The repeating, wooden background of the menu bar */
    tgx_bar_bg = man->GetTgx(man->GetDirectory() + "gfx/1280r.tgx").lock();
    /* The right side of the scribe/book */
    tgx_right = man->GetTgx(man->GetDirectory() + "gfx/edge1280r.tgx").lock();
    /* The scribe's facial animation */
    gm1_scribe = man->GetGm1(man->GetDirectory() + "gm/scribe.gm1").lock();
    /* The menu faces */
    gm1_face = man->GetGm1(man->GetDirectory() + "gm/face800-blank.gm1").lock();
    gm1_icons = man->GetGm1(man->GetDirectory() + "gm/interface_buttons.gm1").lock();

    /* Menu bar (alpha testing) */

    menubar.AllocNewStreaming(240+800+240, 300);
    menubar.LockTexture();

    /* Left side */
    tgx_bar_bg->LockTexture();
    menubar.Copy(*tgx_bar_bg, 0, 100);
    tgx_bar_bg->UnlockTexture();

    /* Menu face */
    auto face = gm1_face->GetTextureAtlas().lock();
    SDL_Rect offset = face->Get(0);
    face->LockTexture();
    menubar.Copy(*face, 240, 300-144, &offset);
    face->UnlockTexture();

    /* Scribe */
    auto scribe = gm1_scribe->GetTextureAtlas().lock();
    offset = scribe->Get(0);
    scribe->LockTexture();
    menubar.Copy(*scribe, 240+705, 100, &offset);
    scribe->UnlockTexture();

    /* Right side */
    tgx_right->LockTexture();
    menubar.Copy(*tgx_right, 240+800, 100);
    tgx_right->UnlockTexture();

    menubar.UnlockTexture();

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

    /*
    Texture &pole = *gm1_maypole->GetTextureAtlas().lock();
    SDL_Rect rect = gm1_maypole->GetTextureAtlas().lock()->Get(manager->GetFrame(maypole));
    manager->Render(pole, mult * rect.w - manager->CamX(), mult * rect.h - manager->CamY(), mult * rect.w, mult * rect.h, &rect);
    */

    Texture &anim = *gm1_anim->GetTextureAtlas().lock();
    SDL_Rect rect = gm1_anim->GetTextureAtlas().lock()->Get(10);
    manager->Render(anim, mult * 240 - manager->CamX(), mult * 240 - manager->CamY(), mult * rect.w, mult * rect.h, &rect);

    rect = gm1_anim->GetTextureAtlas().lock()->Get(12);
    manager->Render(anim, mult * 280 - manager->CamX(), mult * 260 - manager->CamY(), mult * rect.w, mult * rect.h, &rect);

    rect = gm1_anim->GetTextureAtlas().lock()->Get(13);
    manager->Render(anim, mult * 250 - manager->CamX(), mult * 275 - manager->CamY(), mult * rect.w, mult * rect.h, &rect);

    Texture &church = *gm1_churches->GetTextureAtlas().lock();
    rect = gm1_churches->GetTextureAtlas().lock()->Get(0);
    manager->Render(church, mult * 450 - manager->CamX(), mult * 180 - manager->CamY(), mult * rect.w, mult * rect.h, &rect);

    /* Menu bar */
    manager->Render(menubar, 0.0, 0.6, 1.0, 0.4);

    UpdateCamera();
}

void GameMap::UpdateCamera() {
    if(ml) manager->MoveLeft();
    if(mr) manager->MoveRight();
    if(mu) manager->MoveUp();
    if(md) manager->MoveDown();
}

void GameMap::onEventReceive(Keyboard &keyEvent) {
    if(keyEvent.GetType() == KEYBOARD_KEYDOWN) {
        switch(keyEvent.Key().sym) {
        case SDLK_LEFT: ml = true; break;
        case SDLK_RIGHT: mr = true; break;
        case SDLK_UP: mu = true; break;
        case SDLK_DOWN: md = true; break;
        case SDLK_SPACE: {
            if(mult == 1) mult = 2;
            else mult = 1;
        }break;
        default: break;
        }
    }else if(keyEvent.GetType() == KEYBOARD_KEYUP) {
        switch(keyEvent.Key().sym) {
        case SDLK_LEFT: ml = false; break;
        case SDLK_RIGHT: mr = false; break;
        case SDLK_UP: mu = false; break;
        case SDLK_DOWN: md = false; break;
        default: break;
        }

        manager->ResetMomentum();
    }
}

void GameMap::onEventReceive(Mouse &mouseEvent) {
    if(mouseEvent.GetType() == MOUSE_BUTTONDOWN) {
        int64_t px = (mouseEvent.GetPosX() + manager->CamX()) / 30;
        int64_t py = (mouseEvent.GetPosY() + manager->CamY()) / 16;
        int64_t index = px + py * DIM;

        if(index < tiles.size()) {
        }
    }
}
