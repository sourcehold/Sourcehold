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

                Camera();
                ~Camera();

                inline ZoomLevel GetZoomLevel() { return zoomLevel; }
            private:
                ZoomLevel zoomLevel;
                uint32_t positionX, positionY;
            protected:
        };
    }
}
