#pragma once

template <typename T>
struct Vector3 {
  T x;
  T y;
  T z;

  template <typename F>
  friend Vector3<F>& operator+=(Vector3<F>& l, const Vector3<F>& r);
  template <typename F>
  friend Vector3<F>& operator-=(Vector3<F>& l, const Vector3<F>& r);
  template <typename F>
  friend Vector3<F>& operator*=(Vector3<F>& l, const Vector3<F>& r);
  template <typename F>
  friend Vector3<F>& operator/=(Vector3<F>& l, const Vector3<F>& r);
  template <typename F>
  friend Vector3<F>& operator+=(Vector3<F>& l, const F& r);
  template <typename F>
  friend Vector3<F>& operator-=(Vector3<F>& l, const F& r);
  template <typename F>
  friend Vector3<F>& operator*=(Vector3<F>& l, const F& r);
  template <typename F>
  friend Vector3<F>& operator/=(Vector3<F>& l, const F& r);

  template <typename F>
  friend Vector3<F> operator+(const Vector3<F>& l, const Vector3<F>& r);
  template <typename F>
  friend Vector3<F> operator-(const Vector3<F>& l, const Vector3<F>& r);
  template <typename F>
  friend Vector3<F> operator*(const Vector3<F>& l, const Vector3<F>& r);
  template <typename F>
  friend Vector3<F> operator/(const Vector3<F>& l, const Vector3<F>& r);
  template <typename F>
  friend Vector3<F> operator+(const Vector3<F>& l, const F& r);
  template <typename F>
  friend Vector3<F> operator-(const Vector3<F>& l, const F& r);
  template <typename F>
  friend Vector3<F> operator*(const Vector3<F>& l, const F& r);
  template <typename F>
  friend Vector3<F> operator/(const Vector3<F>& l, const F& r);

  template <typename OT>
  operator Vector3<OT>() {
    // only allow conversions between arithmetic types
    static_assert(std::is_arithmetic_v<T>);
    static_assert(std::is_arithmetic_v<OT>);
    return Vector3<OT>{static_cast<OT>(x), static_cast<OT>(y)};
  }
};

namespace Details {
template <typename T, typename F>
Vector3<T>& calc(Vector3<T>& l, const Vector3<T>& r, F f) {
  l.x = f(l.x, r.x);
  l.y = f(l.y, r.y);
  l.z = f(l.z, r.z);
  return l;
}

template <typename T, typename F>
Vector3<T>& calc(Vector3<T>& l, const T& r, F f) {
  l.x = f(l.x, r);
  l.y = f(l.y, r);
  l.z = f(l.z, r);
  return l;
}

}  // namespace Details

template <typename F>
Vector3<F>& operator+=(Vector3<F>& l, const Vector3<F>& r) {
  return Details::calc(l, r, std::plus<F>());
}
template <typename F>
Vector3<F>& operator-=(Vector3<F>& l, const Vector3<F>& r) {
  return Details::calc(l, r, std::minus<F>());
}

template <typename F>
Vector3<F>& operator*=(Vector3<F>& l, const Vector3<F>& r) {
  return Details::calc(l, r, std::multiplies<F>());
}

template <typename F>
Vector3<F>& operator/=(Vector3<F>& l, const Vector3<F>& r) {
  return Details::calc(l, r, std::divides<F>());
}
template <typename F>
Vector3<F>& operator+=(Vector3<F>& l, const F& r) {
  return Details::calc(l, r, std::plus<F>());
}
template <typename F>
Vector3<F>& operator-=(Vector3<F>& l, const F& r) {
  return Details::calc(l, r, std::minus<F>());
}

template <typename F>
Vector3<F>& operator*=(Vector3<F>& l, const F& r) {
  return Details::calc(l, r, std::multiplies<F>());
}

template <typename F>
Vector3<F>& operator/=(Vector3<F>& l, const F& r) {
  return Details::calc(l, r, std::divides<F>());
}

template <typename F>
Vector3<F> operator+(const Vector3<F>& l, const Vector3<F>& r) {
  auto result = l;
  return result += r;
}
template <typename F>
Vector3<F> operator-(const Vector3<F>& l, const Vector3<F>& r) {
  auto result = l;
  return result -= r;
}

template <typename F>
Vector3<F> operator*(const Vector3<F>& l, const Vector3<F>& r) {
  auto result = l;
  return result *= r;
}

template <typename F>
Vector3<F> operator/(const Vector3<F>& l, const Vector3<F>& r) {
  auto result = l;
  return result /= r;
}

template <typename F>
Vector3<F> operator+(const Vector3<F>& l, const F& r) {
  auto result = l;
  return result += r;
}
template <typename F>
Vector3<F> operator-(const Vector3<F>& l, const F& r) {
  auto result = l;
  return result -= r;
}

template <typename F>
Vector3<F> operator*(const Vector3<F>& l, const F& r) {
  auto result = l;
  return result *= r;
}

template <typename F>
Vector3<F> operator/(const Vector3<F>& l, const F& r) {
  auto result = l;
  return result /= r;
}
