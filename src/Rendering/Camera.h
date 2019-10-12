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

            void Update(double dt);

            void ZoomOut();
            void ZoomIn();

            void SetPos(int x, int y);
            void TranslateCam(int x, int y);

            void SetBounds(SDL_Rect bounds);
        public:
            Rotation rotation = UP;
            ZoomLevel zoomLevel = ZOOM_FAR;
            int positionX = 0, positionY = 0;
            float accX = 0.0f, accY = 0.0f;
            double start = 0.0;
        protected:
            const uint8_t zoomStep = 2;
            SDL_Rect bounds;
        };
    }
}
