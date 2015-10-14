#include "assignment5/Assignment5.h"
#include "common/core.h"

std::shared_ptr<Camera> Assignment5::CreateCamera() const
{
    return std::make_shared<Camera>(glm::vec2(1280.f, 720.f), 65.f);
}

std::shared_ptr<Scene> Assignment5::CreateScene() const
{
    std::shared_ptr<Scene> newScene = std::make_shared<Scene>();
    return newScene;
}

std::shared_ptr<class Sampler> Assignment5::CreateSampler() const
{
    return std::make_shared<RandomSampler>();
}

int Assignment5::GetSamplesPerPixel() const
{
    return 4;
}

bool Assignment5::NotifyNewPixelSample(glm::vec3 inputSampleColor, int sampleIndex)
{
    return true;
}