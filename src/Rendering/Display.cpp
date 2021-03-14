#include <vector>
#include "Rendering/Display.h"
#include "System/Logger.h"
#include "System/Config.h"
using namespace Sourcehold;
using namespace System;
SDL_Window *_window;
bool _fullscreen, _nograb;
int _width, _height;
static int ResizeEventWatcher(void *data, SDL_Event *event) {
  if (event->type == SDL_WINDOWEVENT &&
      event->window.event == SDL_WINDOWEVENT_RESIZED) {
    SDL_Window *win = SDL_GetWindowFromID(event->window.windowID);
    if (win == _window) {
      SDL_GetWindowSize(win, &_width, &_height);
    }
  }
  return 0;
}
bool Rendering::InitDisplay(const std::string &title, int width, int height,
                            int index, bool fullscreen, bool noborder,
                            bool nograb, bool resize) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    Logger::error(RENDERING) << SDL_GetError() << std::endl;
    return false;
  }
  int param = SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS;
  _fullscreen = fullscreen;
  _nograb = nograb;
  if (fullscreen) {
    param |= SDL_WINDOW_FULLSCREEN;
  }
  if (noborder) {
    param |= SDL_WINDOW_BORDERLESS;
  }
  if (resize) {
    param |= SDL_WINDOW_RESIZABLE;
  }
  // Select display //
  int displays = SDL_GetNumVideoDisplays();
  if (index >= displays) index = 0;
  auto displayBounds = std::vector<SDL_Rect>(displays);
  for (int i = 0; i < displays; i++) {
    displayBounds[i] = {0, 0, 0, 0};
    SDL_GetDisplayBounds(i, &displayBounds[i]);
  }
#if defined(SOURCEHOLD_ANDROID) || defined(SOURCEHOLD_IOS)
  // On Android and iOS, only use the native resolution //
  if (width < 0 || height < 0) {
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(index, &mode);
    width = mode.w;
    height = mode.h;
    // Flip width and height to force landscape orientation //
    if (height > width) {
      std::swap(width, height);
    }
  }
#endif  // defined(SOURCEHOLD_ANDROID) || defined(SOURCEHOLD_IOS)
  _window = SDL_CreateWindow(
      title.c_str(),
      displayBounds[index].x + (displayBounds[index].w / 2) - width / 2,
      displayBounds[index].y + (displayBounds[index].h / 2) - height / 2, width,
      height, param);
  if (!_window) {
    Logger::error(RENDERING)
        << "Unable to create SDL2 window: " << SDL_GetError() << std::endl;
  }
  if (!nograb) {
    SDL_SetWindowGrab(_window, SDL_TRUE);
  }
  _width = width;
  _height = height;
  SDL_AddEventWatch(ResizeEventWatcher, nullptr);
  return true;
}
void Rendering::DestroyDisplay() {
  SDL_DestroyWindow(_window);
  SDL_Quit();
}
void Rendering::ToggleFullscreen() {
  int err =
      SDL_SetWindowFullscreen(_window, _fullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
  if (err < 0) {
    Logger::error(RENDERING)
        << "Unable to switch to fullscreen: " << SDL_GetError() << std::endl;
  }
  _fullscreen = !_fullscreen;
}
void Rendering::GrabMouse() {
  if (!_nograb && SDL_GetWindowGrab(_window) == SDL_FALSE) {
    SDL_SetWindowGrab(_window, SDL_TRUE);
  }
}
void Rendering::ReleaseMouse() {
  if (SDL_GetWindowGrab(_window) == SDL_TRUE) {
    SDL_SetWindowGrab(_window, SDL_FALSE);
  }
}
bool Rendering::IsDisplayOpen() { return true; }
void Rendering::ErrorMessageBox(const std::string &title,
                                const std::string &msg) {
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), msg.c_str(),
                           _window);
}
int Rendering::GetWidth() { return _width; }
int Rendering::GetHeight() { return _height; }
int Rendering::GetMouseX() {
  int p;
  SDL_GetMouseState(&p, NULL);
  return p;
}
int Rendering::GetMouseY() {
  int p;
  SDL_GetMouseState(NULL, &p);
  return p;
}
void Rendering::MouseOn() { SDL_ShowCursor(SDL_ENABLE); }
void Rendering::MouseOff() { SDL_ShowCursor(SDL_DISABLE); }
SDL_Window *Rendering::GetWindow() { return _window; }
