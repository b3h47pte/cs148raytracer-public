#include "common/Scene/Lights/Area/AreaLight.h"

AreaLight::AreaLight(const glm::vec2& size):
    samplesToUse(4), lightSize(size)
{
    sampler = make_unique<JitterColorSampler>();
    sampler->SetGridSize(glm::ivec3(2, 2, 1));
}

void AreaLight::ComputeSampleRays(std::vector<Ray>& output, glm::vec3 origin, glm::vec3 normal) const
{
    origin += normal * LARGE_EPSILON;
    std::random_device rd;
    std::unique_ptr<SamplerState> sampleState = sampler->CreateSampler(rd, samplesToUse, 2);
    for (int i = 0; i < samplesToUse; ++i) {
        glm::vec3 sample = sampler->ComputeSampleCoordinate(*sampleState.get()) - 0.5f;
        sample.x *= lightSize.x;
        sample.y *= lightSize.y;
        sample.z = 0.f;

        const glm::vec3 lightPosition = glm::vec3(GetObjectToWorldMatrix() * glm::vec4(sample, 1.f));
        const glm::vec3 rayDirection = glm::normalize(lightPosition - origin);
        const float distanceToOrigin = glm::distance(origin, lightPosition);
        output.emplace_back(origin, rayDirection, distanceToOrigin);
    }
    
}

float AreaLight::ComputeLightAttenuation(glm::vec3 origin) const
{
    const glm::vec3 lightToPoint = glm::normalize(origin - glm::vec3(GetPosition()));
    if (glm::dot(lightToPoint, glm::vec3(GetForwardDirection())) < -SMALL_EPSILON) {
        return 0.f;
    }
    return 1.f / static_cast<float>(samplesToUse);
}

void AreaLight::GenerateRandomPhotonRay(Ray& ray) const
{
}

void AreaLight::SetSamplerAttributes(glm::ivec3 inputGridSize, int numSamples)
{
    sampler->SetGridSize(inputGridSize);
    samplesToUse = numSamples;
}