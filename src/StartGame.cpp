#include "StartGame.h"
#include "Engine/Engine.h"
#include "Logger.h"
#include <iostream>

void LoadingScreen() {}

int StartGame() {
  using namespace Sourcehold::System;
  try {
    Logger::message(GAME) << "Initializing Engine .." << std::endl;
    Engine::GetInstance();
    Logger::message(GAME) << "Entering MainLoop .." << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
