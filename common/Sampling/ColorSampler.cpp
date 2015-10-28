#include "common/Sampling/ColorSampler.h"

ColorSampler::ColorSampler()
{
}

void ColorSampler::InitializeSampler(Application* app, Scene* inputScene)
{
    storedApp = app;
    storedScene = inputScene;
}

std::unique_ptr<SamplerState> ColorSampler::CreateSampler(std::random_device& randomDevice, const int maxSamples, const int dimensions) const
{
    return std::move(make_unique<SamplerState>(randomDevice, maxSamples, dimensions));
}

glm::vec3 ColorSampler::ComputeSamplesAndColor(const int maxSamples, const int dimensions, std::function<glm::vec3(glm::vec3)> colorComputer) const
{
    std::random_device randomDevice;
    std::unique_ptr<SamplerState> newState = CreateSampler(randomDevice, maxSamples, dimensions);

    glm::vec3 finalColor;
    for (int i = 0; i < maxSamples; ++i) {
        // Compute normalized sample. 
        glm::vec3 sampleCoordinates = ComputeSampleCoordinate(*newState.get());

        // Compute sample color.
        glm::vec3 sampleColor = colorComputer(sampleCoordinates);
        finalColor += sampleColor;
        ++newState->samplesComputed;

        if (NotifyColorSampleForEarlyExit(*newState.get(), sampleColor)) {
            break;
        }

        newState->colorHistory.push_back(sampleColor);
        
    }
    finalColor /= static_cast<float>(newState->samplesComputed);
    return finalColor;
}

glm::vec3 ColorSampler::ComputeSampleCoordinate(SamplerState& state) const
{
    glm::vec3 sample;
    for (int i = 0; i < 3; ++i) {
        sample[i] = GenerateRandomNumber(state);
    }
    return sample;
}

float ColorSampler::GenerateRandomNumber(SamplerState& state) const
{
    return static_cast<float>(state.dist(state.gen));
}

bool ColorSampler::NotifyColorSampleForEarlyExit(SamplerState& state, glm::vec3 inColor) const
{
    return false;
}
