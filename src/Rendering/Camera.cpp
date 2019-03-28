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
    TranslateCam(speed * momentumX, speed * momentumY);
}
