#pragma once

#include <string>
#include <cinttypes>

#include <SDL.h>

namespace Sourcehold {
namespace Rendering {
bool InitDisplay(const std::string &title, int width, int height, int index = 0,
                 bool fullscreen = false, bool noborder = false,
                 bool nograb = false, bool resize = false);
void DestroyDisplay();
void ToggleFullscreen();
void GrabMouse();
void ReleaseMouse();
bool IsDisplayOpen();
void ErrorMessageBox(const std::string &title, const std::string &msg);

int GetWidth();
int GetHeight();
int GetMouseX();
int GetMouseY();
void MouseOn();
void MouseOff();

SDL_Window *GetWindow();
}  // namespace Rendering
}  // namespace Sourcehold
