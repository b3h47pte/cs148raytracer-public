#include "common/Scene/Geometry/Ray/Ray.h"

Ray::Ray(glm::vec3 inputPosition, glm::vec3 inputDirection, float inputMaxT):
    rayDirection(glm::normalize(inputDirection)), maxT(inputMaxT)
{
    position = glm::vec4(inputPosition, 1.f);
}

glm::vec4 Ray::GetForwardDirection() const
{
    return glm::vec4(GetRayDirection(), 0.f);
}

glm::vec3 Ray::GetRayDirection() const
{
    return rayDirection;
}

float Ray::GetMaxT() const
{
    return maxT;
}
