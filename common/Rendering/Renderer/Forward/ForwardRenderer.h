#pragma once

#include "common/Rendering/Renderer.h"

class ForwardRenderer : public Renderer
{
public:
    ForwardRenderer(std::shared_ptr<class Scene> scene, std::shared_ptr<class ColorSampler> sampler);
    virtual void InitializeRenderer() override;
    glm::vec3 ComputeSampleColor(const struct IntersectionState& intersection, const class Ray& fromCameraRay) const override;
};

