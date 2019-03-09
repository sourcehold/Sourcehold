#include <GameManager.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Audio;
using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

GameManager::GameManager(GameOptions &opt) :
    AnimationHandler(),
    SoundManager(),
    opt(opt),
    Display()
{
    Open(
        "Sourcehold version " SOURCEHOLD_VERSION_STRING,
        opt.width,
        opt.height,
        opt.fullscreen,
        opt.noborder
    );

    eventHandler = std::make_shared<EventHandler>();

    SoundManager::Init();

    running = true;
}

GameManager::GameManager(const GameManager &manager) :
    AnimationHandler(manager),
    SoundManager(manager),
    opt(manager.opt),
    Display()
{
    this->running = manager.running;
}

GameManager::~GameManager() {

}

bool GameManager::Running() {
    GameManager::Update();
    return running;
}

void GameManager::Update() {
    if(!IsOpen() || !eventHandler->FetchEvents()) running = false;

    AnimationHandler::Update();

    time = SDL_GetTicks() / 1000.0;
}
