#pragma once

namespace Sourcehold {
    namespace Rendering {
        template<class T> struct Rect {
        public:
            T x, y, w, h;
            Rect(T x, T y, T w, T h) { this->x = x; this->y = y; this->w = w; this->h = h;  }
            Rect() = default;
        };

        template<class T> struct Line {
        public:
            T x1, y1, x2, y2;
            Line(T x1, T y1, T x2, T y2) { this->x1 = x1; this->y1 = y1; this->x2 = x2; this->y2 = y2;  }
            Line() = default;
        };

        template<class T> struct Vector2 {
        public:
            T x, y;
            Vector2(T x, T y) { this->x = x; this->y = y; }
            Vector2() = default;
        };
    }
}
