#pragma once
#include "common/Sampling/Sampler.h"
#include "common/Sampling/Random/RandomSampler.h"

class JitterSampler : public Sampler
{
public:
    JitterSampler();
    virtual void ResetSamplerState();
    virtual float Sample1D(float rangeMin, float rangeMax);
    virtual glm::vec2 Sample2D(glm::vec2 rangeMin, glm::vec2 rangeMax);
    virtual glm::vec3 Sample3D(glm::vec3 rangeMin, glm::vec3 rangeMax);
    virtual void FinishSample();

    void Initialize(const glm::ivec3 inputGridSize, int inputMaxSamples);

private:

    int GetCurrentCell() const;

    RandomSampler internalSampler;
    glm::ivec3 gridSize;
    int maxSamples;
    int samplesPerCell;
    int processedSamples;
};