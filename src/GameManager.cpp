#include <GameManager.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Audio;
using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

GameManager::GameManager(GameOptions &opt) :
    AnimationHandler(),
    Display(),
    opt(opt)
{
    Open(
        "Sourcehold version " SOURCEHOLD_VERSION_STRING,
        opt.width,
        opt.height,
        opt.fullscreen,
        opt.noborder
    );

    InitOpenAL();

    eventHandler = std::make_shared<EventHandler>();
    
    running = true;
}

GameManager::~GameManager() {
    DestroyOpenAL();
}

bool GameManager::Running() {
    GameManager::Update();
    return running;
}

void GameManager::Update() {
    if(!IsOpen() || !eventHandler->FetchEvents()) running = false;

    AnimationHandler::Update();
    Renderer::Update();

    time = SDL_GetTicks() / 1000.0;
}

