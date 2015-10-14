#pragma once
#include "common/Sampling/Sampler.h"

class RandomSampler : public Sampler
{
public:
    virtual void ResetSamplerState();
    virtual float Sample1D(float rangeMin, float rangeMax);
    virtual glm::vec2 Sample2D(glm::vec2 rangeMin, glm::vec2 rangeMax);
    virtual glm::vec3 Sample3D(glm::vec3 rangeMin, glm::vec3 rangeMax);
};