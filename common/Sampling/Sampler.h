#pragma once

#include "common/common.h"
#include <random>

class Sampler
{
public:
    virtual ~Sampler();

    virtual void ResetSamplerState() = 0;
    virtual float Sample1D(float rangeMin, float rangeMax) = 0;
    virtual glm::vec2 Sample2D(glm::vec2 rangeMin, glm::vec2 rangeMax) = 0;
    virtual glm::vec3 Sample3D(glm::vec3 rangeMin, glm::vec3 rangeMax) = 0;

protected:
    std::random_device randomDevice;
};