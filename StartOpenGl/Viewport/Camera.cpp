#include "stdafx.h"

#include "Camera.h"

namespace Viewport {
    Camera::Camera() :
        _right(_transform[0]),
        _up(_transform[1]),
        _forward(_transform[2]),
        _translation(_transform[3])
    {
    }

    void Camera::MoveForward(float augment)
    {
        _translation += _forward * augment;
    }
}