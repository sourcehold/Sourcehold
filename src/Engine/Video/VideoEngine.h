#ifndef VIDEO_ENGINE_H_
#define VIDEO_ENGINE_H_
#include <SDL2/SDL.h>
class VideoEngine {
 public:
  VideoEngine();
  ~VideoEngine();

 private:
  void InitSDL();
  void CreateWindow();
  void CreateRenderer();
  SDL_Renderer* renderer_;
  SDL_Window* window_;
};
#endif  // VIDEO_ENGINE_H_
