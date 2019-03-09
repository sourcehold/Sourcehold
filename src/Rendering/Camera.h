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

                Camera(uint32_t x = 0, uint32_t y = 0);
                ~Camera();

                inline ZoomLevel GetZoomLevel() { return zoomLevel; }
                inline uint32_t CamX() { return positionX; }
                inline uint32_t CamY() { return positionY; }
                inline void SetCamX(uint32_t pos) { positionX = pos; }
                inline void SetCamY(uint32_t pos) { positionY = pos; }
                inline void TranslateCam(int32_t x, int32_t y) { positionX += x; positionY += y; }
            private:
                ZoomLevel zoomLevel;
                uint32_t positionX, positionY;
            protected:
        };
    }
}
