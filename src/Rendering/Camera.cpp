#include <algorithm>

#include "Rendering/Camera.h"
#include "Rendering/Display.h"

#include "System/Config.h"

#include "GameManager.h"

using namespace Sourcehold;
using namespace Rendering;
using namespace Game;

void Camera::MoveLeft()
{
    acc_x_ = std::max(acc_x_ - 1.0f, -10.0f);
}

void Camera::MoveRight()
{
    acc_x_ = std::min(acc_x_ + 1.0f, 10.0f);
}

void Camera::MoveUp()
{
    acc_y_ = std::max(acc_y_ - 1.0f, -10.0f);
}

void Camera::MoveDown()
{
    acc_y_ = std::min(acc_y_ + 1.0f, 10.0f);
}

void Camera::Stop()
{
    acc_x_ = acc_y_ = 0.0f;
}

void Camera::Update(float dt)
{
#if defined(SOURCEHOLD_ANDROID) && defined(INVERT_CAM_ON_ANDROID)
    positionX -= 60.0 * dt * accX;
    positionY -= 50.0 * dt * accY;

    // TODO
    accX *= 0.96f;
    accY *= 0.96f;
#else
    pos_x_ += static_cast<int>(100.0f * dt * acc_x_);
    pos_y_ += static_cast<int>(100.0f * dt * acc_y_);
#endif

    pos_x_ = std::max(bounds_.x, pos_x_);
    pos_y_ = std::max(bounds_.y, pos_y_);

    pos_x_ = std::min(bounds_.x + bounds_.w - GetWidth(), pos_x_);
    pos_y_ = std::min(bounds_.x + bounds_.h - GetHeight(), pos_y_);
}

void Camera::ZoomOut()
{
    zoom_level_ = ZOOM_FAR;
}

void Camera::ZoomIn()
{
    zoom_level_ = ZOOM_NEAR;
}

void Camera::SetPos(int x, int y)
{
    pos_x_ = x;
    pos_y_ = y;
}

void Camera::TranslateCam(int x, int y)
{
    pos_x_ += x;
    pos_y_ += y;
}

void Camera::SetBounds(SDL_Rect bounds)
{
    bounds = bounds_;
}
