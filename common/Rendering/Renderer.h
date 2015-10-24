#pragma once

#include "common/common.h"

class Renderer : public std::enable_shared_from_this<Renderer>
{
public:
    Renderer(std::shared_ptr<class Scene> scene, std::shared_ptr<class ColorSampler> sampler);
    virtual ~Renderer();

    virtual void InitializeRenderer() = 0;
    
    virtual glm::vec3 ComputeSampleColor(const struct IntersectionState& intersection, const class Ray& fromCameraRay) const = 0;
protected:
    std::shared_ptr<class Scene> storedScene;
    std::shared_ptr<class ColorSampler> storedSampler;
};