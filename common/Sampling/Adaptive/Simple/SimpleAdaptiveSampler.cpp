#include "common/Sampling/Adaptive/Simple/SimpleAdaptiveSampler.h"

SimpleAdaptiveSampler::SimpleAdaptiveSampler() :
    earlyExitThreshold(SMALL_EPSILON), minimumEarlyExitSamples(1)
{
}

std::unique_ptr<SamplerState> SimpleAdaptiveSampler::CreateSampler(std::random_device& randomDevice, const int maxSamples, const int dimensions) const
{
    std::unique_ptr<SimpleAdaptiveSamplerState> state = make_unique<SimpleAdaptiveSamplerState>(randomDevice, maxSamples, dimensions);
    state->internalState = internalSampler->CreateSampler(randomDevice, maxSamples, dimensions);
    return std::move(state);
}

glm::vec3 SimpleAdaptiveSampler::ComputeSampleCoordinate(SamplerState& state) const
{
    return internalSampler->ComputeSampleCoordinate(state);
}

void SimpleAdaptiveSampler::InitializeSampler(class Application* app, class Scene* inputScene)
{
    ColorSampler::InitializeSampler(app, inputScene);
    internalSampler->InitializeSampler(app, inputScene);
}

void SimpleAdaptiveSampler::SetInternalSampler(std::shared_ptr<ColorSampler> inputSampler)
{
    internalSampler = std::move(inputSampler);
}

bool SimpleAdaptiveSampler::NotifyColorSampleForEarlyExit(SamplerState& state, glm::vec3 inColor) const
{
    if (static_cast<int>(state.colorHistory.size()) < minimumEarlyExitSamples) {
        return false;
    }

    glm::vec3 averageColor;
    for (size_t i = 0; i < state.colorHistory.size(); ++i) {
        averageColor += state.colorHistory[i];
    }
    averageColor /= static_cast<float>(state.colorHistory.size());

    // ASSIGNMENT 5 (OPTIONAL): Modify this line to change the adaptive condition.
    if (glm::distance(averageColor, inColor) < earlyExitThreshold) {
        return true;
    }

    return false;
}

void SimpleAdaptiveSampler::SetEarlyExitParameters(float threshold, int minSampleCount)
{
    earlyExitThreshold = threshold;
    minimumEarlyExitSamples = minSampleCount;
}