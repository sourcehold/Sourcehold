#include <Building.h>
#include <World.h>
#include <Parsers/TgxFile.h>
#include <Parsers/Gm1File.h>

using namespace Sourcehold::Game;

World::World(std::shared_ptr<GameManager> mgr) :
    GameMap(mgr),
    manager(mgr),
    menubar(mgr),
    EventConsumer<Keyboard>(mgr->GetHandler()),
    EventConsumer<Mouse>(mgr->GetHandler())
{
    /* The repeating, wooden background of the menu bar */
    tgx_bar_bg = mgr->GetTgx(mgr->GetDirectory() + "gfx/1280r.tgx").lock();
    /* The right side of the scribe/book */
    tgx_right = mgr->GetTgx(mgr->GetDirectory() + "gfx/edge1280r.tgx").lock();
    /* The scribe's facial animation */
    gm1_scribe = mgr->GetGm1(mgr->GetDirectory() + "gm/scribe.gm1").lock();
    /* The menu faces */
    gm1_face = mgr->GetGm1(mgr->GetDirectory() + "gm/face800-blank.gm1").lock();
    gm1_icons = mgr->GetGm1(mgr->GetDirectory() + "gm/interface_buttons.gm1").lock();

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

    auto icon = gm1_icons->GetTextureAtlas().lock();
    icon->LockTexture();

    offset = icon->Get(7);
    menubar.Copy(*icon, 264, 265, &offset);
    offset = icon->Get(10);
    menubar.Copy(*icon, 300, 265, &offset);
    offset = icon->Get(13);
    menubar.Copy(*icon, 336, 265, &offset);
    offset = icon->Get(16);
    menubar.Copy(*icon, 372, 265, &offset);
    offset = icon->Get(19);
    menubar.Copy(*icon, 408, 265, &offset);
    offset = icon->Get(22);
    menubar.Copy(*icon, 444, 265, &offset);

    offset = icon->Get(25);
    menubar.Copy(*icon, 753, 172, &offset);
    offset = icon->Get(27);
    menubar.Copy(*icon, 753, 210, &offset);
    offset = icon->Get(29);
    menubar.Copy(*icon, 753, 242, &offset);
    offset = icon->Get(68);
    menubar.Copy(*icon, 753, 268, &offset);

    icon->UnlockTexture();

    menubar.UnlockTexture();

    manager->SetBounds({ 15, 8, 160 * 30 - 15, 90 * 16 - 8});
    manager->TranslateCam(15, 8);
}

World::~World() {
}

int World::Play() {
    while(manager->Running()) {
        manager->Clear();
        manager->StartTimer();

        UpdateCamera();

        GameMap::Render();

        RenderMenubar();

        manager->EndTimer();
        manager->Flush();
    }

    return 0;
}

void World::RenderMenubar() {
    /* Menu bar */
    manager->Render(menubar, 0.0, 0.6, 1.0, 0.4);
}

void World::UpdateCamera() {
    if(ml) manager->MoveLeft();
    if(mr) manager->MoveRight();
    if(mu) manager->MoveUp();
    if(md) manager->MoveDown();
}

void World::onEventReceive(Keyboard &keyEvent) {
    if(keyEvent.GetType() == KEYBOARD_KEYDOWN) {
        switch(keyEvent.Key().sym) {
        case SDLK_LEFT: ml = true; break;
        case SDLK_RIGHT: mr = true; break;
        case SDLK_UP: mu = true; break;
        case SDLK_DOWN: md = true; break;
        case SDLK_SPACE: break;
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

void World::onEventReceive(Mouse &mouseEvent) {
    if(mouseEvent.GetType() == MOUSE_BUTTONDOWN) {
        int64_t px = (mouseEvent.GetPosX() + manager->CamX()) / 30;
        int64_t py = (mouseEvent.GetPosY() + manager->CamY()) / 16;
    }
}

