#ifndef ENGINE_H_
#define ENGINE_H_
#include "GameOptions.h"
#include "Video/VideoEngine.h"
#include "Audio/AudioEngine.h"
#include "Assets/AssetLoader.h"

class Engine {
 public:
  Engine(const GameOptions& options);
  ~Engine() = default;
  static void InitLogger(bool force_color);

 private:
  VideoEngine video_engine_;
  AudioEngine audio_engine_;
  AssetLoader asset_loader_;
};
#endif  // ENGINE_H_
