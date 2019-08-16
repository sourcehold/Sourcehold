#include <algorithm>

#include "Rendering/Camera.h"

using namespace Sourcehold;
using namespace Rendering;

static SDL_Rect _bounds;
static Rotation rotation = UP;
static ZoomLevel zoomLevel = ZOOM_FAR;
static const uint8_t zoomStep = 2;
static int32_t positionX = 0, positionY = 0;
static int32_t momentumX = 0, momentumY = 0;
static int32_t maxMomentum = 8;
static int32_t speed = 2;

SDL_Rect Rendering::GetVisibleArea()
{
    SDL_Rect rect = { };

    /* TODO */
    //int8_t zoomFactor = (ZOOM_NEAR ? zoomStep : 1);

    return rect;
}

void Rendering::SetBounds(SDL_Rect bounds)
{
    _bounds = bounds;
}

void Rendering::MoveLeft()
{
    if(momentumX > -maxMomentum) momentumX--;
}

void Rendering::MoveRight()
{
    if(momentumX < maxMomentum) momentumX++;
}

void Rendering::MoveUp()
{
    if(momentumY > -maxMomentum) momentumY--;
}

void Rendering::MoveDown()
{
    if(momentumY < maxMomentum) momentumY++;
}

void Rendering::UpdateCamera()
{
    /* TODO */
    int32_t px = speed * momentumX, py = speed * momentumY;
    TranslateCam(px, py);

    if(positionX < _bounds.x) positionX = _bounds.x;
    if(positionY < _bounds.y) positionY = _bounds.y;

    SDL_Rect area = GetVisibleArea();
}

ZoomLevel Rendering::GetZoomLevel()
{
    return zoomLevel;
}

Rotation Rendering::GetRotation()
{
    return rotation;
}

void Rendering::ZoomOut()
{
    zoomLevel = ZOOM_FAR;
}

void Rendering::ZoomIn()
{
    zoomLevel = ZOOM_NEAR;
}

int32_t Rendering::CamX()
{
    return positionX;
}

int32_t Rendering::CamY()
{
    return positionY;
}

void Rendering::SetCamX(int32_t pos)
{
    positionX = pos;
}

void Rendering::SetCamY(int32_t pos)
{
    positionY = pos;
}

void Rendering::ResetMomentum()
{
    momentumX = momentumY = 0;
}

void Rendering::SetCamPos(int32_t x, int32_t y)
{
    positionX = x;
    positionY = y;
}

void Rendering::TranslateCam(int32_t x, int32_t y)
{
    positionX += x;
    positionY += y;
}

int32_t Rendering::CamStepX()
{
    return speed * momentumX;
}

int32_t Rendering::CamStepY()
{
    return speed * momentumY;
}
