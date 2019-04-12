#include <algorithm>

#include <Rendering/Camera.h>

using namespace Sourcehold::Rendering;

Camera::Camera(int32_t x, int32_t y) {
    positionX = x;
    positionY = y;
    momentumX = 0;
    momentumY = 0;
    maxMomentum = 8;
    speed = 2;
}

Camera::~Camera() {
}

SDL_Rect Camera::GetVisibleArea() {
    SDL_Rect rect;

    /* TODO */
    int8_t zoomFactor = (ZOOM_NEAR ? zoomStep : 1);

    return rect;
}

void Camera::SetBounds(SDL_Rect bounds) {
    this->bounds = bounds;
}

void Camera::MoveLeft() {
    if(momentumX > -maxMomentum) momentumX--;
}

void Camera::MoveRight() {
    if(momentumX < maxMomentum) momentumX++;
}

void Camera::MoveUp() {
    if(momentumY > -maxMomentum) momentumY--;
}

void Camera::MoveDown() {
    if(momentumY < maxMomentum) momentumY++;
}

void Camera::Update() {
    /* TODO */
    int32_t px = speed * momentumX, py = speed * momentumY;
    TranslateCam(px, py);

    if(positionX < bounds.x) positionX = bounds.x;
    if(positionY < bounds.y) positionY = bounds.y;

    SDL_Rect area = GetVisibleArea();
}

