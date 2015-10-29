#include "common/Scene/Geometry/Ray/Ray.h"

Ray::Ray() :
    rayDirection(glm::vec3(0.f, 0.f, -1.f)), maxT(std::numeric_limits<float>::max())
{
    DIAGNOSTICS_STAT(DiagnosticsType::RAYS_CREATED);
    position = glm::vec4(0.f, 0.f, 0.f, 1.f);
}

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

void Ray::SetMaxT(float input)
{
    maxT = input;
}

glm::vec3 Ray::GetRayPosition(float t) const
{
    return glm::vec3(position) + t * rayDirection;
}

void Ray::SetRayMask(uint64_t objectId)
{
    traceMask[objectId] = true;
}

bool Ray::IsObjectMasked(uint64_t objectId) const
{
    try {
        return traceMask.at(objectId);
    } catch (...) {
        return false;
    }
}