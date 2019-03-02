#include <GameManager.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Audio;
using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

GameManager::GameManager(GameOptions &opt) :
    AnimationHandler(),
    SoundManager(),
    opt(opt)
{
    renderer = std::make_shared<Display>();
    renderer->Open("Sourcehold version " SOURCEHOLD_VERSION_STRING, opt.width, opt.height, opt.fullscreen);

    SoundManager::Init();
}

GameManager::GameManager(const GameManager &manager) :
    AnimationHandler(manager),
    SoundManager(manager),
    opt(manager.opt)
{
    
}

GameManager::~GameManager() {

}

void GameManager::Update() {
    AnimationHandler::Update();
}

bool GameManager::Running() {
    return renderer->IsOpen();
}
