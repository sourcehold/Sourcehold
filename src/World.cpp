#include <Building.h>
#include <World.h>
#include <Parsers/TgxFile.h>
#include <Parsers/Gm1File.h>

using namespace Sourcehold::Game;

World::World(std::shared_ptr<GameManager> mgr) :
    GameMap(mgr),
    manager(mgr),
    EventConsumer<Keyboard>(mgr->GetHandler()),
    EventConsumer<Mouse>(mgr->GetHandler()),
    ui_disk(mgr),
    ui_info(mgr),
    ui_delete(mgr),
    ui_revert(mgr)
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
        case SDLK_SPACE:
            if(manager->GetZoomLevel() == Camera::ZOOM_NEAR) manager->ZoomOut();
            else manager->ZoomIn();
            break;
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

