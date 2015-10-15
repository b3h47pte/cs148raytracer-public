#pragma once

#include "common/Scene/Camera/Camera.h"

class PerspectiveCamera : public Camera
{
public:
    // inputFov is in degrees. 
    PerspectiveCamera(float aspectRatio, float inputFov);
    virtual std::shared_ptr<class Ray> GenerateRayForNormalizedCoordinates(glm::vec2 coordinate) const override;

private:
    float aspectRatio;
    float fov; // fov is stored as radians
};