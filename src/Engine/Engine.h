#ifndef ENGINE_H_
#define ENGINE_H_
#include "GameOptions.h"
#include "Video/VideoEngine.h"
#include "Audio/AudioEngine.h"
#include "Assets/AssetLoader.h"

class Engine {
 public:
  // Meyer Singleton
  static Engine& GetInstance() {
    InitLogger(game_options_gk.force_color_);
    static Engine engine;
    return engine;
  }

  VideoEngine video_engine_;
  AudioEngine audio_engine_;
  AssetLoader asset_loader_;

 private:
  Engine() = default;
  Engine(const Engine& other) = delete;
  Engine& operator=(const Engine& other) = delete;
  Engine& operator=(Engine&& other) = delete;
  ~Engine() = default;

  static void InitLogger(bool force_color);
};

#endif  // ENGINE_H_
