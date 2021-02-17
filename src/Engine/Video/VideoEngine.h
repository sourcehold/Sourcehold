#ifndef VIDEO_ENGINE_H_
#define VIDEO_ENGINE_H_
#include "GameOptions.h"
#include <SDL2/SDL.h>
class VideoEngine {
 public:
  VideoEngine(const GameOptions& options);
  ~VideoEngine();

 private:
  void InitSDL();
  void CreateWindow(const GameOptions& options);
  void CreateRenderer();
  SDL_Renderer* renderer_;
  SDL_Window* window_;
};
#endif  // VIDEO_ENGINE_H_
