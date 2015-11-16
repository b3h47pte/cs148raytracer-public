#pragma once

#include "common/Scene/Lights/Light.h"
#include "common/Sampling/Jitter/JitterColorSampler.h"

class AreaLight : public Light
{
public:
    AreaLight(const glm::vec2& size);

    virtual void ComputeSampleRays(std::vector<Ray>& output, glm::vec3 origin, glm::vec3 normal) const override;
    virtual float ComputeLightAttenuation(glm::vec3 origin) const override;

    virtual void GenerateRandomPhotonRay(Ray& ray) const override;

    // Sampler Attributes
    void SetSamplerAttributes(glm::ivec3 inputGridSize, int numSamples);
private:
    std::unique_ptr<JitterColorSampler> sampler;
    int samplesToUse;
    glm::vec2 lightSize;
};