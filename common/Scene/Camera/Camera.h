#pragma once

#include "common/Scene/SceneObject.h"

class Camera : public SceneObject
{
public:
    Camera(glm::vec2 inputResolution, float inputFov);

    glm::vec2 GetResolution() const;
private:
    glm::vec2 resolution;
    float fov;
    float aspectRatio;
};