#pragma once

#include "common/common.h"
#include <random>

// WARNING: DO NOT MULTI-THREAD THE SAMPLER
// EACH THREAD SHOULD HAVE ITS OWN SAMPLER. IF YOU REALLY NEED IT, YOU'LL HAVE TO INSERT LOCKS AND STUFF. :(
class Sampler
{
public:
    virtual ~Sampler();

    virtual void ResetSamplerState() = 0;
    virtual float Sample1D(float rangeMin, float rangeMax) = 0;
    virtual glm::vec2 Sample2D(glm::vec2 rangeMin, glm::vec2 rangeMax) = 0;
    virtual glm::vec3 Sample3D(glm::vec3 rangeMin, glm::vec3 rangeMax) = 0;
    virtual void FinishSample() {}

protected:
    std::random_device randomDevice;
};