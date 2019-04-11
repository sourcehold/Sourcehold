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
                ZOOM_FAR, // Default zoom, no scaling
                ZOOM_NEAR, // Some upscaling
                ZOOM_BATTLEFIELD // Downscaling to see the whole map
            };

            enum Rotation : uint8_t {
                UP,
                LEFT,
                DOWN,
                RIGHT
            };

            Camera(int32_t x = 0, int32_t y = 0);
            ~Camera();

            /**
             * Calculate the currently visible area
             * in world coordinates.
             */
            SDL_Rect GetVisibleArea();
            void SetBounds(SDL_Rect bounds);

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
            inline Rotation GetRotation() { return rotation; }
            inline void ZoomOut() { zoomLevel = ZOOM_FAR; }
            inline void ZoomIn() { zoomLevel = ZOOM_NEAR; }
            inline int32_t CamX() { return positionX; }
            inline int32_t CamY() { return positionY; }
            inline void SetCamX(int32_t pos) { positionX = pos; }
            inline void SetCamY(int32_t pos) { positionY = pos; }
            inline void ResetMomentum() { momentumX = momentumY = 0; }
            inline void TranslateCam(int32_t x, int32_t y) { positionX += x; positionY += y; }
            inline int32_t CamStepX() { return speed * momentumX; }
            inline int32_t CamStepY() { return speed * momentumY; }
        private:
            SDL_Rect bounds;
            Rotation rotation = UP;
            ZoomLevel zoomLevel = ZOOM_FAR;
            const static uint8_t zoomStep = 2;
            int32_t positionX, positionY;
            int32_t momentumX, momentumY;
            int32_t maxMomentum;
            int32_t speed;
        protected:
        };
    }
}
