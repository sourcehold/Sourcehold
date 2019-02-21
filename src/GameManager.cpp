#include <GameManager.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Audio;
using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

GameManager::GameManager() : Display(), SoundManager() {
    this->Init();
}

GameManager::GameManager(const GameManager &manager) : Display(manager), SoundManager(manager) {
    
}

GameManager::~GameManager() {

}

void GameManager::Init() {
    Open("Sourcehold version " SOURCEHOLD_VERSION_STRING, 800, 600, false);
    //Fullscreen();
    SoundManager::Init();
}
