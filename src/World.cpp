#include "Building.h"
#include "Entity.h"
#include "World.h"

#include "Parsers/TgxFile.h"
#include "Parsers/Gm1File.h"

#include "Rendering/Font.h"
#include "Rendering/Camera.h"

using namespace Sourcehold::Game;
using namespace Sourcehold::GUI;

World::World() :
    GameMap(WORLD_160), // TODO
    EventConsumer<Keyboard>(),
    EventConsumer<Mouse>(),
    IngameGUI()
{
    LoadMenuAssets();

    SetBounds({ 15, 8, 160 * 30 - 15, 90 * 16 - 8});
    SetCamPos(15, 8);
}

World::~World()
{
}

UIState World::Play()
{
    while(Running()) {
        ClearDisplay();

        UpdateCamera();

        GameMap::Render();
        if (!IngameGUI::Render()) break;

        RenderText(L"Sourcehold version " SOURCEHOLD_VERSION_STRING, 1, 1, FONT_SMALL);

        FlushDisplay();
        SyncDisplay();
    }

    return EXIT_GAME;
}

void World::UpdateCamera()
{
    if(scroll.left) MoveLeft();
    if(scroll.right) MoveRight();
    if(scroll.up) MoveUp();
    if(scroll.down) MoveDown();
}

void World::onEventReceive(Keyboard &keyEvent)
{
    if(keyEvent.GetType() == KEYDOWN) {
        switch(keyEvent.Key().sym) {
        case SDLK_LEFT:
            scroll.left.shouldScroll = true;
            scroll.left.setByKeyboard = true;
            break;
        case SDLK_RIGHT:
            scroll.right.shouldScroll = true;
            scroll.right.setByKeyboard = true;
            break;
        case SDLK_UP:
            scroll.up.shouldScroll = true;
            scroll.up.setByKeyboard = true;
            break;
        case SDLK_DOWN:
            scroll.down.shouldScroll = true;
            scroll.down.setByKeyboard = true;
            break;
        case SDLK_SPACE:
            if(GetZoomLevel() == ZOOM_NEAR) ZoomOut();
            else ZoomIn();
            break;
        default:
            break;
        }
    }
    else if(keyEvent.GetType() == KEYUP) {
        switch(keyEvent.Key().sym) {
        case SDLK_LEFT:
            scroll.left.shouldScroll = false;
            scroll.left.setByKeyboard = false;
            break;
        case SDLK_RIGHT:
            scroll.right.shouldScroll = false;
            scroll.right.setByKeyboard = false;
            break;
        case SDLK_UP:
            scroll.up.shouldScroll = false;
            scroll.up.setByKeyboard = false;
            break;
        case SDLK_DOWN:
            scroll.down.shouldScroll = false;
            scroll.down.setByKeyboard = false;
            break;
        default:
            break;
        }

        ResetMomentum();
    }
}

void World::onEventReceive(Mouse &mouseEvent)
{
    if(mouseEvent.GetType() == MOTION) {
        const int scrollThreshold = 2;

        int x = mouseEvent.GetPosX();
        int y = mouseEvent.GetPosY();

        bool shouldReset = false;

        if(x < scrollThreshold) {
            scroll.left.shouldScroll = true;
            scroll.left.setByMouse = true;
        }
        else if(scroll.left.setByMouse) {
            scroll.left.shouldScroll = false;
            scroll.left.setByMouse = false;
            shouldReset = true;
        }

        if(x > GetWidth() - scrollThreshold) {
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

        if(y > GetHeight() - scrollThreshold) {
            scroll.down.shouldScroll = true;
            scroll.down.setByMouse = true;
        }
        else if(scroll.down.setByMouse) {
            scroll.down.shouldScroll = false;
            scroll.down.setByMouse = false;
            shouldReset = true;
        }

        if(shouldReset) {
            ResetMomentum();
        }
    }
}
