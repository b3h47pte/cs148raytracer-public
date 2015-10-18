#include "common/Rendering/Renderer/Forward/ForwardRenderer.h"

ForwardRenderer::ForwardRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<Sampler> sampler):
    Renderer(scene, sampler)
{
}

void ForwardRenderer::InitializeRenderer()
{
}