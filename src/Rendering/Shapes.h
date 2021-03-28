#pragma once

namespace Sourcehold {
namespace Rendering {
template <class T>
struct Rect {
  T x, y, w, h;
};

template <class T>
struct Line {
  T x1, y1, x2, y2;
};

template <class T>
struct Vector2 {
  T x, y;
};
}  // namespace Rendering
}  // namespace Sourcehold
