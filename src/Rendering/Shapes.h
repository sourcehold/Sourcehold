#pragma once

namespace Sourcehold {
    namespace Rendering {
        template<class T> struct Rect {
        public:
            T x, y, w, h;
        };

        template<class T> struct Line {
        public:
            T x1, y1, x2, y2;
        };
    }
}
