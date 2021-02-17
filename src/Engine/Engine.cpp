#include "Engine.h"
#include "System/Logger.h"
void Engine::InitLogger(bool force_color) {
  using namespace Sourcehold::System;

#if SOURCEHOLD_UNIX == 1 && !defined(SOURCEHOLD_ANDROID)
  Logger::SetColorOutput(true);
#else
  Logger::SetColorOutput(false);
#endif

  Logger::SetColorOutput(force_color);
}

Engine::Engine(const GameOptions& options)
    : video_engine_(options), asset_loader_(options.data_directory_path_) {
  audio_engine_.muted_ = options.nosound_;
}

