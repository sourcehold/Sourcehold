#include <GameManager.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Sound;
using namespace Sourcehold::System;
using namespace Sourcehold::Rendering;

GameManager::GameManager() : Display(), Sound() {
    this->Init();
}

GameManager::GameManager(const GameManager &manager) : Display(manager), Sound(manager) {
    
}

GameManager::~GameManager() {

}

void GameManager::Init() {
    OpenWindowed("Sourcehold version " SOURCEHOLD_VERSION_STRING, 800, 600);
    Sound::Init();
}
