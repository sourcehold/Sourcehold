#include <GameManager.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Audio;
using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

GameManager::GameManager() :
    AnimationHandler(),
    Display(),
    SoundManager()
{
    this->Init();
}

GameManager::GameManager(const GameManager &manager) :
    AnimationHandler(manager),
    Display(manager),
    SoundManager(manager)
{
    
}

GameManager::~GameManager() {

}

bool GameManager::Running() {
    return Display::IsOpen();
}

void GameManager::Update() {
    AnimationHandler::Update();
}

void GameManager::Init() {
    Open("Sourcehold version " SOURCEHOLD_VERSION_STRING, 800, 600, false);
    //Fullscreen();
    SoundManager::Init();
}
