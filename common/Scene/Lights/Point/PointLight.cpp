#include "common/Scene/Lights/Point/PointLight.h"


void PointLight::ComputeSampleRays(std::vector<Ray>& output, glm::vec3 origin) const
{
    const glm::vec3 lightPosition = glm::vec3(GetPosition());
    const glm::vec3 rayDirection = glm::normalize(lightPosition - origin);
    const float distanceToOrigin = glm::distance(origin, lightPosition);
    output.emplace_back(origin + rayDirection * LARGE_EPSILON, rayDirection, distanceToOrigin);
}

float PointLight::ComputeLightAttenuation(glm::vec3 origin) const
{
    return 1.f;
}