#include <algorithm>

#include "Rendering/Camera.h"
#include "Rendering/Display.h"

#include "System/Config.h"

#include "GameManager.h"

using namespace Sourcehold;
using namespace Rendering;
using namespace Game;

void Camera::MoveLeft() {
  accX = std::max(accX - 1.0, -10.0);
}

void Camera::MoveRight() {
  accX = std::min(accX + 1.0, 10.0);
}

void Camera::MoveUp() {
  accY = std::max(accY - 1.0, -10.0);
}

void Camera::MoveDown() {
  accY = std::min(accY + 1.0, 10.0);
}

void Camera::Stop() {
  accX = accY = 0.0f;
}

void Camera::Update(double dt) {
#if defined(SOURCEHOLD_ANDROID) && defined(INVERT_CAM_ON_ANDROID)
  positionX -= 60.0 * dt * accX;
  positionY -= 50.0 * dt * accY;

  // TODO
  accX *= 0.96f;
  accY *= 0.96f;
#else
  positionX += 100.0 * dt * accX;
  positionY += 100.0 * dt * accY;
#endif

  positionX = std::max(bounds.x, positionX);
  positionY = std::max(bounds.y, positionY);

  auto display_size = Display::Size();
  positionX = std::min(bounds.x + bounds.w - display_size.x, positionX);
  positionY = std::min(bounds.x + bounds.h - display_size.y, positionY);
}

void Camera::ZoomOut() {
  zoomLevel = ZOOM_FAR;
}

void Camera::ZoomIn() {
  zoomLevel = ZOOM_NEAR;
}

void Camera::SetPos(int x, int y) {
  positionX = x;
  positionY = y;
}

void Camera::TranslateCam(int x, int y) {
  positionX += x;
  positionY += y;
}

void Camera::SetBounds(SDL_Rect bounds) {
  this->bounds = bounds;
}
