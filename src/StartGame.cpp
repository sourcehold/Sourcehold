#include "StartGame.h"
#include "Engine/Engine.h"
#include <iostream>

int StartGame(const GameOptions& options) {
  try {
    Engine::InitLogger(options.force_color_);
    Engine engine(options);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
