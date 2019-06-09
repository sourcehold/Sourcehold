#pragma once

#include <cinttypes>
#include <SDL.h>

namespace Sourcehold
{
	namespace Rendering
	{
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
		void UpdateCamera();

		ZoomLevel GetZoomLevel();
		Rotation GetRotation();
		void ZoomOut();
		void ZoomIn();
		int32_t CamX();
		int32_t CamY();
		void SetCamX(int32_t pos);
		void SetCamY(int32_t pos);
		void ResetMomentum();
		void SetCamPos(int32_t x, int32_t y);
		void TranslateCam(int32_t x, int32_t y);
		int32_t CamStepX();
		int32_t CamStepY();
	}
}
