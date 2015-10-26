#pragma once

#include "common/common.h"
#include <random>

struct SamplerState
{
    SamplerState(std::random_device& device, int inputMax, int inputDim) :
        maxSamples(inputMax), dimensions(inputDim), samplesComputed(0), gen(device()), dist(0, 1)
    {
    }

    std::vector<glm::vec3> colorHistory;
    const int maxSamples;
    const int dimensions;
    int samplesComputed;

    std::mt19937 gen;
    std::uniform_real_distribution<> dist;
};

class ColorSampler : public std::enable_shared_from_this<ColorSampler>
{
public:
    ColorSampler();

    virtual std::unique_ptr<SamplerState> CreateSampler(std::random_device& randomDevice, const int maxSamples, const int dimensions) const;
    virtual void InitializeSampler(class Application* app, class Scene* inputScene);

    virtual glm::vec3 ComputeSamplesAndColor(const int maxSamples, const int dimensions, std::function<glm::vec3(glm::vec3)> colorComputer) const;
    virtual glm::vec3 ComputeSampleCoordinate(SamplerState& state) const;
protected:
    virtual float GenerateRandomNumber(SamplerState& state) const;
    virtual bool NotifyColorSampleForEarlyExit(SamplerState& state, glm::vec3 inColor) const;

    class Application* storedApp;
    class Scene* storedScene;
};