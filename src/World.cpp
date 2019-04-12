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
    if(scroll.left) manager->MoveLeft();
    if(scroll.right) manager->MoveRight();
    if(scroll.up) manager->MoveUp();
    if(scroll.down) manager->MoveDown();
}

void World::onEventReceive(Keyboard &keyEvent) {
    if(keyEvent.GetType() == KEYBOARD_KEYDOWN) {
        switch(keyEvent.Key().sym) {
        case SDLK_LEFT: scroll.left.shouldScroll = true; scroll.left.setByKeyboard = true; break;
        case SDLK_RIGHT: scroll.right.shouldScroll = true; scroll.right.setByKeyboard = true; break;
        case SDLK_UP: scroll.up.shouldScroll = true; scroll.up.setByKeyboard = true; break;
        case SDLK_DOWN: scroll.down.shouldScroll = true; scroll.down.setByKeyboard = true; break;
        case SDLK_SPACE:
            if(manager->GetZoomLevel() == Camera::ZOOM_NEAR) manager->ZoomOut();
            else manager->ZoomIn();
            break;
        default: break;
        }
    }else if(keyEvent.GetType() == KEYBOARD_KEYUP) {
        switch(keyEvent.Key().sym) {
        case SDLK_LEFT: scroll.left.shouldScroll = false; scroll.left.setByKeyboard = false; break;
        case SDLK_RIGHT: scroll.right.shouldScroll = false; scroll.right.setByKeyboard = false;  break;
        case SDLK_UP: scroll.up.shouldScroll = false; scroll.up.setByKeyboard = false;  break;
        case SDLK_DOWN: scroll.down.shouldScroll = false; scroll.down.setByKeyboard = false;  break;
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
    else if(mouseEvent.GetType() == MOUSE_MOTION) {
        auto scrollThreshold = 2;

        auto x = mouseEvent.GetPosX();
        auto y = mouseEvent.GetPosY();

        auto shouldReset = false;

        if(x < scrollThreshold) {
            scroll.left.shouldScroll = true;
            scroll.left.setByMouse = true;
        }
        else if(scroll.left.setByMouse) {
            scroll.left.shouldScroll = false;
            scroll.left.setByMouse = false;
            shouldReset = true;
        }

        if(x > manager->GetWidth() - scrollThreshold) {
            scroll.right.shouldScroll = true;
            scroll.right.setByMouse = true;
        }
        else if(scroll.right.setByMouse) {
            scroll.right.shouldScroll = false;
            scroll.right.setByMouse = false;
            shouldReset = true;
        }

        if(y < scrollThreshold) {
            scroll.up.shouldScroll = true;
            scroll.up.setByMouse = true;
        }
        else if(scroll.up.setByMouse) {
            scroll.up.shouldScroll = false;
            scroll.up.setByMouse = false;
            shouldReset = true;
        }

        if(y > manager->GetHeight() - scrollThreshold) {
            scroll.down.shouldScroll = true;
            scroll.down.setByMouse = true;
        }
        else if(scroll.down.setByMouse) {
            scroll.down.shouldScroll = false;
            scroll.down.setByMouse = false;
            shouldReset = true;
        }

        if(shouldReset) {
            manager->ResetMomentum();
        }
    }
}

