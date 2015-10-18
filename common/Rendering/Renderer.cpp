#include "common/Rendering/Renderer.h"
#include "common/Scene/Scene.h"
#include "common/Sampling/Sampler.h"

Renderer::Renderer(std::shared_ptr<Scene> scene, std::shared_ptr<Sampler> sampler):
    storedScene(scene), storedSampler(sampler)
{
}

Renderer::~Renderer()
{
}

glm::vec3 Renderer::ComputeSampleColor(const struct IntersectionState& intersection) const
{
    return glm::vec3();
}