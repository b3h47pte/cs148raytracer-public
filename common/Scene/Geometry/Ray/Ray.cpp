#include "common/Scene/Geometry/Ray/Ray.h"

Ray::Ray() :
    rayDirection(glm::vec3(0.f, 0.f, -1.f)), maxT(std::numeric_limits<float>::max())
{
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

bool Ray::IsObjectMasked(uint64_t objectId)
{
    return traceMask[objectId];
}

glm::vec3 Ray::RefractRay(const glm::vec3& normal, float n1, float& n2) const
{
    const float eta = n1 / n2;
    const float NdI = glm::dot(normal, GetRayDirection());
    const float cosTheta1 = std::abs(NdI);
    const float tirCheck = std::pow(eta, 2.f) * (1.f - std::pow(cosTheta1, 2.f));
    if (tirCheck - 1.f > SMALL_EPSILON) {
        n2 = n1;
        return glm::reflect(GetRayDirection(), (NdI > SMALL_EPSILON) ? -1.f * normal : normal);
    }
    const float cosTheta2 = std::sqrt(1.f - tirCheck);
    const glm::vec3 refractionDir = eta * GetRayDirection() + (eta * cosTheta1 - cosTheta2) * normal;
    return refractionDir;
}