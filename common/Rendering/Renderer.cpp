#include "common/Rendering/Renderer.h"
#include "common/Scene/Scene.h"
#include "common/Sampling/ColorSampler.h"

Renderer::Renderer(std::shared_ptr<Scene> scene, std::shared_ptr<ColorSampler> sampler) :
    storedScene(scene), storedSampler(sampler)
{
}

Renderer::~Renderer()
{
}