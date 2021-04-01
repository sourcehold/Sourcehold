#include <vector>
#include <cassert>
#include "Rendering/Display.h"
#include "System/Config.h"
#include "SDL/SDLBackend.h"

#include "System/Logger.h"
namespace Sourcehold {
using namespace SDL;
namespace Rendering {

bool Display::Fullscreen() {
  return SDL_GetWindowFlags(SDLBackend::Window()) & SDL_WINDOW_FULLSCREEN;
}

void Display::Fullscreen(bool val) {
  auto param = val ? SDL_WINDOW_FULLSCREEN : 0u;
  SDL_SetWindowFullscreen(SDLBackend::Window(), param);
}

void Display::ToggleFullscreen() {
  Fullscreen(!Fullscreen());
}

bool Display::WindowGrab() {
  return SDL_GetWindowGrab(SDLBackend::Window());
}

void Display::WindowGrab(bool val) {
  auto param = val ? SDL_TRUE : SDL_FALSE;
  SDL_SetWindowGrab(SDLBackend::Window(), param);
}

void Display::ToggleWindowGrab() {
  WindowGrab(!WindowGrab());
}

bool Display::Borderless() {
  return SDL_GetWindowFlags(SDLBackend::Window()) & SDL_WINDOW_BORDERLESS;
}

void Display::Borderless(bool val) {
  auto param = val ? SDL_FALSE : SDL_TRUE;
  SDL_SetWindowBordered(SDLBackend::Window(), param);
}

void Display::ToggleBorderless() {
  Borderless(!Borderless());
}

bool Display::Resizable() {
#if defined(SOURCEHOLD_IOS) || defined(SOURCEHOLD_ANDROID)
  return false;
#else   // !SOURCEHOLD_IOS || !SOURCEHOLD_ANDROID
  return SDL_GetWindowFlags(SDLBackend::Window()) & SDL_WINDOW_RESIZABLE;
#endif  // SOURCEHOLD_IOS || SOURCEHOLD_ANDROID
}

void Display::Resizable(bool val) {
#if defined(SOURCEHOLD_IOS) || defined(SOURCEHOLD_ANDROID)
  SDL_SetWindowResizable(SDLBackend::Window(), SDL_FALSE);
#else   // !SOURCEHOLD_IOS || !SOURCEHOLD_ANDROID
  auto param = val ? SDL_TRUE : SDL_FALSE;
  SDL_SetWindowResizable(SDLBackend::Window(), param);
#endif  // SOURCEHOLD_IOS || SOURCEHOLD_ANDROID
}

void Display::ToggleResizable() {
  Resizable(!Resizable());
}

Vector2<int> Display::Size() {
  int width;
  int height;

  SDL_GetWindowSize(SDLBackend::Window(), &width, &height);
  auto result = Vector2<int>{width, height};

#if defined(SOURCEHOLD_IOS) || defined(SOURCEHOLD_ANDROID)
  if (result.y > result.x) {
    std::swap(result.y, result.x);
  }
#endif  // SOURCEHOLD_IOS || SOURCEHOLD_ANDROID
  return result;
}

void Display::Size(Vector2<int> size) {
#if !defined(SOURCEHOLD_IOS) || !defined(SOURCEHOLD_ANDROID)
  SDL_SetWindowSize(SDLBackend::Window(), size.x, size.y);
#endif  // !SOURCEHOLD_IOS || !SOURCEHOLD_ANDROID
}

void Display::Title(const std::string &title) {
  SDL_SetWindowTitle(SDLBackend::Window(), title.c_str());
}

int Display::DisplayIndex() {
  return SDL_GetWindowDisplayIndex(SDLBackend::Window());
}

void Display::DisplayIndex(int index) {
  index = SDL_GetNumVideoDisplays() <= index ? 0 : index;
  SDL_SetWindowPosition(SDLBackend::Window(),
                        SDL_WINDOWPOS_CENTERED_DISPLAY(index),
                        SDL_WINDOWPOS_CENTERED_DISPLAY(index));
}

Vector2<int> Display::MousePosition() {
  int x;
  int y;

  SDL_GetMouseState(&x, &y);
  return {x, y};
}

bool Display::Mouse() {
  return SDL_ShowCursor(SDL_QUERY);
}

void Display::Mouse(bool val) {
  auto param = val ? SDL_ENABLE : SDL_DISABLE;
  SDL_ShowCursor(param);
}

void Display::ToggleMouse() {
  Mouse(!Mouse());
}

void Display::Set(const std::string &title, Vector2<int> size,
                  int display_index, bool fullscreen, bool borderless,
                  bool window_grab, bool resizeable) {
  Title(title);
  Size(size);
  DisplayIndex(display_index);
  Fullscreen(fullscreen);
  Borderless(borderless);
  WindowGrab(window_grab);
  Resizable(resizeable);

  SDL_SetWindowInputFocus(SDLBackend::Window());
  SDL_ShowWindow(SDLBackend::Window());
}

void Display::ErrorMessageBox(const std::string &title,
                              const std::string &msg) {
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), msg.c_str(),
                           SDLBackend::Window());
}

}  // namespace Rendering
}  // namespace Sourcehold
