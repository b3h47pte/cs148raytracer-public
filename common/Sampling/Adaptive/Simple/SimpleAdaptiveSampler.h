#pragma once

#include "common/Sampling/ColorSampler.h"

struct SimpleAdaptiveSamplerState : public SamplerState
{
    SimpleAdaptiveSamplerState(std::random_device& device, int inputMax, int inputDim) :
        SamplerState(device, inputMax, inputDim)
    {
    }

    std::unique_ptr<SamplerState> internalState;
};

class SimpleAdaptiveSampler : public ColorSampler
{
public:
    SimpleAdaptiveSampler();
    void SetInternalSampler(std::shared_ptr<ColorSampler> inputSampler);
    void SetEarlyExitParameters(float threshold, int minSampleCount);

    virtual std::unique_ptr<SamplerState> CreateSampler(std::random_device& randomDevice, const int maxSamples, const int dimensions) const override;
    virtual glm::vec3 ComputeSampleCoordinate(SamplerState& state) const override;

    virtual void InitializeSampler(class Application* app, class Scene* inputScene) override;

protected:
    virtual bool NotifyColorSampleForEarlyExit(SamplerState& state, glm::vec3 inColor) const override;
private:
    std::shared_ptr<ColorSampler> internalSampler;
    float earlyExitThreshold;
    int minimumEarlyExitSamples;
};