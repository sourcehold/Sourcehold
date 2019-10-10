#include <algorithm>

#include "Rendering/Camera.h"
#include "Rendering/Display.h"

#include "GameManager.h"

using namespace Sourcehold;
using namespace Rendering;
using namespace Game;

void Camera::MoveLeft()
{
    accX -= 1.0f;
}

void Camera::MoveRight()
{
    accX += 1.0f;
}

void Camera::MoveUp()
{
    accY -= 1.0f;
}

void Camera::MoveDown()
{
    accY += 1.0f;
}

void Camera::Stop()
{
    accX = accY = 0.0f;
}

void Camera::Update()
{
    // TODO
    
    positionX = std::max(bounds.x, positionX);
    positionY = std::max(bounds.y, positionY);

    positionX = std::min(bounds.x + bounds.w - GetWidth(), positionX);
    positionY = std::min(bounds.x + bounds.h - GetHeight(), positionY);
}

void Camera::ZoomOut()
{
    zoomLevel = ZOOM_FAR;
}

void Camera::ZoomIn()
{
    zoomLevel = ZOOM_NEAR;
}

void Camera::SetPos(int x, int y)
{
    positionX = x;
    positionY = y;
}

void Camera::TranslateCam(int x, int y)
{
    positionX += x;
    positionY += y;
}

void Camera::SetBounds(SDL_Rect bounds)
{
    this->bounds = bounds;
}
