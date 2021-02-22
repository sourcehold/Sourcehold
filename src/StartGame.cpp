#include "StartGame.h"
#include "Engine/Engine.h"
#include "System/Logger.h"
#include <iostream>

int StartGame(const GameOptions& options) {
  using namespace Sourcehold::System;
  try {
    Logger::message(GAME) << "Initializing Engine .." << std::endl;
    auto& engine = Engine::GetInstance();
    Logger::message(GAME) << "Entering MainLoop .." << std::endl;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
