#include <gtest/gtest.h>
#include <random>
#include "Rendering/Shapes.h"
using namespace Sourcehold::Rendering;

static Vector2<int> RandVec() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::binomial_distribution<> d(100, 0.5);

  return {d(gen), d(gen)};
}

TEST(Vector2, EmptyInitializerIsZero) {
  Vector2<int> candidate{};
  ASSERT_EQ(candidate.x, 0);
  ASSERT_EQ(candidate.y, 0);
}

constexpr auto sample_size_k = 10;

TEST(Vector2, Add) {
  for (auto i = 0; i < sample_size_k; ++i) {
    auto a = RandVec();
    auto b = RandVec();
    auto c = a + b;

    ASSERT_EQ(c.x, a.x + b.x);
    ASSERT_EQ(c.y, a.y + b.y);
  }
}

TEST(Vector2, Sub) {
  for (auto i = 0; i < sample_size_k; ++i) {
    auto a = RandVec();
    auto b = RandVec();
    auto c = a - b;

    ASSERT_EQ(c.x, a.x - b.x);
    ASSERT_EQ(c.y, a.y - b.y);
  }
}

TEST(Vector2, Mul) {
  for (auto i = 0; i < sample_size_k; ++i) {
    auto a = RandVec();
    auto b = RandVec();
    auto c = a * b;

    ASSERT_EQ(c.x, a.x * b.x);
    ASSERT_EQ(c.y, a.y * b.y);
  }
}

TEST(Vector2, Div) {
  for (auto i = 0; i < sample_size_k; ++i) {
    auto a = RandVec();
    auto b = RandVec();
    auto c = a / b;

    ASSERT_EQ(c.x, a.x / b.x);
    ASSERT_EQ(c.y, a.y / b.y);
  }
}

TEST(Vector2, AddScalar) {
  for (auto i = 0; i < sample_size_k; ++i) {
    auto a = RandVec();
    auto b = RandVec().x;
    auto c = a + b;

    ASSERT_EQ(c.x, a.x + b);
    ASSERT_EQ(c.y, a.y + b);
  }
}

TEST(Vector2, SubScalar) {
  for (auto i = 0; i < sample_size_k; ++i) {
    auto a = RandVec();
    auto b = RandVec().x;
    auto c = a - b;

    ASSERT_EQ(c.x, a.x - b);
    ASSERT_EQ(c.y, a.y - b);
  }
}

TEST(Vector2, MulScalar) {
  for (auto i = 0; i < sample_size_k; ++i) {
    auto a = RandVec();
    auto b = RandVec().x;
    auto c = a * b;

    ASSERT_EQ(c.x, a.x * b);
    ASSERT_EQ(c.y, a.y * b);
  }
}

TEST(Vector2, DivScalar) {
  for (auto i = 0; i < sample_size_k; ++i) {
    auto a = RandVec();
    auto b = RandVec().x;
    auto c = a / b;

    ASSERT_EQ(c.x, a.x / b);
    ASSERT_EQ(c.y, a.y / b);
  }
}
