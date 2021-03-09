#pragma once

#include <cinttypes>
#include <SDL.h>

namespace Sourcehold {
    namespace Rendering {
        enum ZoomLevel : uint8_t {
            ZOOM_FAR,
            ZOOM_NEAR,
            ZOOM_BATTLEFIELD
        };

        enum Rotation : uint8_t {
            UP,
            LEFT,
            DOWN,
            RIGHT
        };

        class Camera {
        public:
            static Camera& instance() {
                static Camera instance;
                return instance;
            }

            void MoveLeft();
            void MoveRight();
            void MoveUp();
            void MoveDown();
            void Stop();

            void Update(float dt);

            void ZoomOut();
            void ZoomIn();

            void SetPos(int x, int y);
            void TranslateCam(int x, int y);

            void SetBounds(SDL_Rect bounds);

        public:
            Rotation rotation_ = UP;
            ZoomLevel zoom_level_ = ZOOM_FAR;

            int pos_x_ = 0;
            int pos_y_ = 0;

            float acc_x_ = 0.0f;
            float acc_y_ = 0.0f;

            float start_ = 0.0f;

        protected:
            const uint8_t zoom_step_ = 2;
            SDL_Rect bounds_;
        };
    }
}
