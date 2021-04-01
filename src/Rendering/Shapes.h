#pragma once
#include <array>
#include <functional>

namespace Sourcehold {
namespace Rendering {
template <typename T>
struct Rect {
  T x;
  T y;
  T w;
  T h;
};

template <typename T>
struct Line {
  T x1;
  T y1;
  T x2;
  T y2;
};

template <typename T>
struct Vector2 {
  T x;
  T y;

  template <typename F>
  friend Vector2<F>& operator+=(Vector2<F>& l, const Vector2<F>& r);
  template <typename F>
  friend Vector2<F>& operator-=(Vector2<F>& l, const Vector2<F>& r);
  template <typename F>
  friend Vector2<F>& operator*=(Vector2<F>& l, const Vector2<F>& r);
  template <typename F>
  friend Vector2<F>& operator/=(Vector2<F>& l, const Vector2<F>& r);
  template <typename F>
  friend Vector2<F>& operator+=(Vector2<F>& l, const F& r);
  template <typename F>
  friend Vector2<F>& operator-=(Vector2<F>& l, const F& r);
  template <typename F>
  friend Vector2<F>& operator*=(Vector2<F>& l, const F& r);
  template <typename F>
  friend Vector2<F>& operator/=(Vector2<F>& l, const F& r);

  template <typename F>
  friend Vector2<F> operator+(const Vector2<F>& l, const Vector2<F>& r);
  template <typename F>
  friend Vector2<F> operator-(const Vector2<F>& l, const Vector2<F>& r);
  template <typename F>
  friend Vector2<F> operator*(const Vector2<F>& l, const Vector2<F>& r);
  template <typename F>
  friend Vector2<F> operator/(const Vector2<F>& l, const Vector2<F>& r);
  template <typename F>
  friend Vector2<F> operator+(const Vector2<F>& l, const F& r);
  template <typename F>
  friend Vector2<F> operator-(const Vector2<F>& l, const F& r);
  template <typename F>
  friend Vector2<F> operator*(const Vector2<F>& l, const F& r);
  template <typename F>
  friend Vector2<F> operator/(const Vector2<F>& l, const F& r);

  template <typename OT>
  operator Vector2<OT>() {
    return Vector2<OT>{static_cast<OT>(x), static_cast<OT>(y)};
  }
};

namespace Details {
template <typename T, typename F>
Vector2<T>& calc(Vector2<T>& l, const Vector2<T>& r, F f) {
  l.x = f(l.x, r.x);
  l.y = f(l.y, r.y);
  return l;
}

template <typename T, typename F>
Vector2<T>& calc(Vector2<T>& l, const T& r, F f) {
  l.x = f(l.x, r);
  l.y = f(l.y, r);
  return l;
}

}  // namespace Details

template <typename F>
Vector2<F>& operator+=(Vector2<F>& l, const Vector2<F>& r) {
  return Details::calc(l, r, std::plus());
}
template <typename F>
Vector2<F>& operator-=(Vector2<F>& l, const Vector2<F>& r) {
  return Details::calc(l, r, std::minus());
}

template <typename F>
Vector2<F>& operator*=(Vector2<F>& l, const Vector2<F>& r) {
  return Details::calc(l, r, std::multiplies());
}

template <typename F>
Vector2<F>& operator/=(Vector2<F>& l, const Vector2<F>& r) {
  return Details::calc(l, r, std::divides());
}
template <typename F>
Vector2<F>& operator+=(Vector2<F>& l, const F& r) {
  return Details::calc(l, r, std::plus());
}
template <typename F>
Vector2<F>& operator-=(Vector2<F>& l, const F& r) {
  return Details::calc(l, r, std::minus());
}

template <typename F>
Vector2<F>& operator*=(Vector2<F>& l, const F& r) {
  return Details::calc(l, r, std::multiplies());
}

template <typename F>
Vector2<F>& operator/(Vector2<F>& l, const F& r) {
  return Details::calc(l, r, std::divides());
}

template <typename F>
Vector2<F> operator+(const Vector2<F>& l, const Vector2<F>& r) {
  auto result = l;
  return result += r;
}
template <typename F>
Vector2<F> operator-(const Vector2<F>& l, const Vector2<F>& r) {
  auto result = l;
  return result -= r;
}

template <typename F>
Vector2<F> operator*(const Vector2<F>& l, const Vector2<F>& r) {
  auto result = l;
  return result *= r;
}

template <typename F>
Vector2<F> operator/(const Vector2<F>& l, const Vector2<F>& r) {
  auto result = l;
  return result /= r;
}

template <typename F>
Vector2<F> operator+(const Vector2<F>& l, const F& r) {
  auto result = l;
  return result += r;
}
template <typename F>
Vector2<F> operator-(const Vector2<F>& l, const F& r) {
  auto result = l;
  return result -= r;
}

template <typename F>
Vector2<F> operator*(const Vector2<F>& l, const F& r) {
  auto result = l;
  return result *= r;
}

template <typename F>
Vector2<F> operator/(const Vector2<F>& l, const F& r) {
  auto result = l;
  return result /= r;
}

}  // namespace Rendering
}  // namespace Sourcehold
