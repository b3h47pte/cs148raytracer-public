#pragma once

#include "common/Rendering/Renderer.h"

class BackwardRenderer : public Renderer
{
public:
    BackwardRenderer(std::shared_ptr<class Scene> scene, std::shared_ptr<class ColorSampler> sampler);
    virtual void InitializeRenderer() override;
    glm::vec3 ComputeSampleColor(const struct IntersectionState& intersection, const class Ray& fromCameraRay) const override;
};

