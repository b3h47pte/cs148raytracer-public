#include "common/Scene/Camera/Camera.h"

Camera::Camera(glm::vec2 inputResolution, float inputFov):
    resolution(inputResolution), fov(inputFov), aspectRatio(inputResolution.y / inputResolution.x)
{
}

glm::vec2 Camera::GetResolution() const
{
    return resolution;
}