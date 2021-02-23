#include "Engine.h"
#include "Config.h"
#include "Logger.h"
void Engine::InitLogger(bool force_color) {
  using namespace Sourcehold::System;

#if SOURCEHOLD_UNIX == 1 && !defined(SOURCEHOLD_ANDROID)
  Logger::SetColorOutput(true);
#else
  Logger::SetColorOutput(false);
#endif

  Logger::SetColorOutput(force_color);
}
