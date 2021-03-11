#pragma once

// TODO(seidl, github@skrax): remove unnecessary constructors
namespace Sourcehold {
    namespace Rendering {
        template<class T> struct Rect {
        public:
            T x, y, w, h;
            Rect(T x_, T y_, T w_, T h_) { this->x = x_; this->y = y_; this->w = w_; this->h = h_;  }
            Rect() = default;
        };

        template<class T> struct Line {
        public:
            T x1, y1, x2, y2;
            Line(T x1_, T y1_, T x2_, T y2_) { this->x1 = x1_; this->y1 = y1_; this->x2 = x2_; this->y2 = y2_;  }
            Line() = default;
        };

        template<class T> struct Vector2 {
        public:
            T x, y;
            Vector2(T x_, T y_) { this->x = x_; this->y = y_; }
            Vector2() = default;
        };
    }
}
