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

    SoundManager::Init();
}

GameManager::GameManager(const GameManager &manager) :
    AnimationHandler(manager),
    SoundManager(manager),
    opt(manager.opt),
    Display()
{
    
}

GameManager::~GameManager() {

}

bool GameManager::Running() {
    return IsOpen();
}

void GameManager::Update() {
    AnimationHandler::Update();
}
