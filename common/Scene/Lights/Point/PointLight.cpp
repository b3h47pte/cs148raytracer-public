#include "common/Scene/Lights/Point/PointLight.h"


void PointLight::ComputeSampleRays(std::vector<Ray>& output, glm::vec3 origin) const
{
    const glm::vec3 lightPosition = glm::vec3(GetPosition());
    const float distanceToOrigin = glm::distance(origin, lightPosition);
    output.emplace_back(origin, glm::normalize(lightPosition - origin), distanceToOrigin - EPSILON);
}

float PointLight::ComputeLightAttenuation(glm::vec3 origin) const
{
    return 1.f;
}