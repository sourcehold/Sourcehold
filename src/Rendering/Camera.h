#pragma once

#include <cinttypes>

#include <SDL2/SDL.h>

namespace Sourcehold
{
    namespace Rendering
    {
        class Camera
        {
            public:
                enum ZoomLevel : uint8_t {
                    ZOOM_FAR,
                    ZOOM_NEAR
                };

                Camera(int32_t x = 0, int32_t y = 0);
                ~Camera();

                /**
                 * Move the camera on a given axis, this will take
                 * the momentum into consideration
                 */
                void MoveLeft();
                void MoveRight();
                void MoveUp();
                void MoveDown();

                /**
                 * Update the camera position based on momentum and speed
                 */
                void Update();

                inline ZoomLevel GetZoomLevel() { return zoomLevel; }
                inline int32_t CamX() { return positionX; }
                inline int32_t CamY() { return positionY; }
                inline void SetCamX(int32_t pos) { positionX = pos; }
                inline void SetCamY(int32_t pos) { positionY = pos; }
                inline void ResetMomentum() { momentumX = momentumY = 0; }
                inline void TranslateCam(int32_t x, int32_t y) { positionX += x; positionY += y; }
            private:
                ZoomLevel zoomLevel;
                int32_t positionX, positionY;
                int32_t momentumX, momentumY;
                int32_t maxMomentum;
                int32_t speed;
            protected:
        };
    }
}
