#pragma once

#include <string>
#include "Rendering/Shapes.h"

namespace Sourcehold {
namespace Rendering {
class Display {
 public:
  static bool Fullscreen();
  static void Fullscreen(bool);
  static void ToggleFullscreen();

  static bool WindowGrab();
  static void WindowGrab(bool);
  static void ToggleWindowGrab();

  static bool Borderless();
  static void Borderless(bool);
  static void ToggleBorderless();

  static bool Resizable();
  static void Resizable(bool);
  static void ToggleResizable();

  static Vector2<int> Size();
  static void Size(Vector2<int> size);

  static void Title(const std::string &title);

  static int DisplayIndex();
  static void DisplayIndex(int);

  static Vector2<int> MousePosition();
  static bool Mouse();
  static void Mouse(bool);
  static void ToggleMouse();

  static void Set(const std::string &title, Vector2<int> size,
                  int display_index, bool fullscreen, bool borderless,
                  bool window_grab, bool resizeable);

  static void ErrorMessageBox(const std::string &title, const std::string &msg);
};
}  // namespace Rendering
}  // namespace Sourcehold
