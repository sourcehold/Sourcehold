#include <random>
#include <entt/entt.hpp>

#include "Building.h"
#include "World.h"
#include "ECS/ECS.h"
#include "ECS/Manager.h"

#include "Parsers/TgxFile.h"
#include "Rendering/Font.h"

using namespace Sourcehold::Game;
using namespace Sourcehold::GUI;
using namespace Sourcehold::ECS;

World::World() :
    GameMap(WORLD_160), // TODO
    EventConsumer<Keyboard>(),
    EventConsumer<Mouse>()
{
    registry = initializeECS();
    Camera& cam = Camera::instance();
    cam.SetPos(15, 8);
}

World::~World()
{
}

int x, startX = 30;
int paddingX = 4;
int maxX = 140;
int y, startY = 30;
int paddingY = 5;
void placeDebug(entt::registry &registry, EntityType et) {
    spawn(registry, et, x, y);
    x += paddingX;
    if (x > maxX) {
        x = startX;
        y += paddingY;
    }
}

UIState World::Play()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 160);

    placeDebug(registry, EntityType::DEER);
    placeDebug(registry, EntityType::LORD);
    
    placeDebug(registry, EntityType::TREE_CHESTNUT_XL);
    placeDebug(registry, EntityType::TREE_CHESTNUT_XL_FALLING);
    placeDebug(registry, EntityType::TREE_CHESTNUT_XL_RESOURCE);
    placeDebug(registry, EntityType::TREE_CHESTNUT_L);
    placeDebug(registry, EntityType::TREE_CHESTNUT_L_FALLING);
    placeDebug(registry, EntityType::TREE_CHESTNUT_L_RESOURCE);
    placeDebug(registry, EntityType::TREE_CHESTNUT_M);
    placeDebug(registry, EntityType::TREE_CHESTNUT_M_FALLING);
    placeDebug(registry, EntityType::TREE_CHESTNUT_M_RESOURCE);
    placeDebug(registry, EntityType::TREE_CHESTNUT_S);
    placeDebug(registry, EntityType::TREE_CHESTNUT_S_FALLING);
    placeDebug(registry, EntityType::TREE_CHESTNUT_S_RESOURCE);
    placeDebug(registry, EntityType::TREE_CHESTNUT_DEAD);
    placeDebug(registry, EntityType::TREE_CHESTNUT_STUMP);

    placeDebug(registry, EntityType::TREE_BIRCH_XL);
    placeDebug(registry, EntityType::TREE_BIRCH_XL_FALLING);
    placeDebug(registry, EntityType::TREE_BIRCH_XL_RESOURCE);
    placeDebug(registry, EntityType::TREE_BIRCH_L);
    placeDebug(registry, EntityType::TREE_BIRCH_L_FALLING);
    placeDebug(registry, EntityType::TREE_BIRCH_L_RESOURCE);
    placeDebug(registry, EntityType::TREE_BIRCH_DEAD);
    placeDebug(registry, EntityType::TREE_BIRCH_STUMP);

    placeDebug(registry, EntityType::TREE_SHRUB1_RED);
    placeDebug(registry, EntityType::TREE_SHRUB1_GREEN);
    placeDebug(registry, EntityType::TREE_SHRUB2);

    placeDebug(registry, EntityType::TREE_APPLE_BUD);
    placeDebug(registry, EntityType::TREE_APPLE_FLOWER);
    placeDebug(registry, EntityType::TREE_APPLE_FRUIT);
    placeDebug(registry, EntityType::TREE_APPLE_EMPTY);
    placeDebug(registry, EntityType::TREE_APPLE_STUMP);

    double previous = GetTime();
    while(Running()) {
        // Timing is good enough for now (TODO obviously) //
        double now = GetTime();
        double frame = now - previous;
        previous = now;

        while(frame > 0.0) {
            double delta = std::min(frame, 1.0 / 60.0);
            UpdateCamera(frame);
            for (Unit* unit : units) unit->Update(frame);
            frame -= delta;
        }

        ClearDisplay();

        GameMap::Render();

        ECS::Manager::GetInstance().Update(registry);
        ECS::Manager::GetInstance().Render(registry);

        if (!gui.Render()) break;

        RenderText(L"Sourcehold version " SOURCEHOLD_VERSION_STRING, 1, 1, FONT_SMALL);

        FlushDisplay();
    }

    for (Unit *unit : units) {
        delete unit;
    }

    return EXIT_GAME;
}

void World::UpdateCamera(double dt)
{
    Camera& cam = Camera::instance();
    cam.SetBounds({ 15, 8, 160 * 30 - 15, 87 * 16 }); // TODO

    if(scroll.left) cam.MoveLeft();
    if(scroll.right)cam.MoveRight();
    if(scroll.up)   cam.MoveUp();
    if(scroll.down) cam.MoveDown();

    cam.Update(dt);
}

void World::onEventReceive(Keyboard &keyEvent)
{
    Camera& cam = Camera::instance();

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
            if(cam.zoomLevel == ZOOM_NEAR) cam.ZoomOut();
            else cam.ZoomIn();
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

        cam.Stop();
    }
}

void World::onEventReceive(Mouse &mouseEvent)
{
    Camera& cam = Camera::instance();

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
            cam.Stop();
        }
    }
    else if (mouseEvent.GetType() == BUTTONDOWN) {
        int x = (cam.positionX + mouseEvent.GetPosX()) / 30;
        int y = (cam.positionY + mouseEvent.GetPosY()) / 15;

        spawn(registry, EntityType::LORD, x, y);
    }
}

void World::onEventReceive(Touch& touchEvent)
{
    Camera& cam = Camera::instance();

    if (touchEvent.GetType() == FINGERMOTION) {
        float dx = touchEvent.GetDx();
        float dy = touchEvent.GetDy();

        // To screen coord
        int sdx = (int)(dx * (float)GetWidth() * 0.5f + 0.5f),
            sdy = (int)(dy * (float)GetHeight() * 0.5f + 0.5f);

        const float CAMERA_ACC_MINMAX = 10.0f;

        // Move horizontally
        cam.accX += sdx;
        cam.accX = std::max<float>(-CAMERA_ACC_MINMAX, cam.accX);
        cam.accX = std::min<float>(CAMERA_ACC_MINMAX, cam.accX);

        // Move vertically
        cam.accY += sdy;
        cam.accY = std::max<float>(-CAMERA_ACC_MINMAX, cam.accY);
        cam.accY = std::min<float>(CAMERA_ACC_MINMAX, cam.accY);
    }
    else if (touchEvent.GetType() == FINGERDOWN) {
        float x = touchEvent.GetY();
        float y = touchEvent.GetX();

        // To screen coord
        int sx = (int)(x * (float)GetWidth() * 0.5f + 0.5f),
            sy = (int)(y * (float)GetHeight() * 0.5f + 0.5f);

        int wpX = (cam.positionX + sx) / 30;
        int wpY = (cam.positionY + sy) / 15;
    }
}
