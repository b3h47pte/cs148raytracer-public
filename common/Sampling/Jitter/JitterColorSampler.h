#pragma once

#include "common/Sampling/ColorSampler.h"

struct JitterSamplerState : public SamplerState
{
    JitterSamplerState(std::random_device& device, int inputMax, int inputDim) :
        SamplerState(device, inputMax, inputDim), samplesPerCell(0)
    {
    }

    int samplesPerCell;
};

class JitterColorSampler : public ColorSampler
{
public:
    void SetGridSize(glm::ivec3 inputGridSize);

    virtual std::unique_ptr<SamplerState> CreateSampler(std::random_device& randomDevice, const int maxSamples, const int dimensions) const override;
    virtual glm::vec3 ComputeSampleCoordinate(SamplerState& state) const override;
private:
    glm::ivec3 gridSize;
};