#pragma once

#include "common/Application.h"

class Assignment6 : public Application
{
public:
    virtual std::shared_ptr<class Camera> CreateCamera() const override;
    virtual std::shared_ptr<class Scene> CreateScene() const override;
    virtual std::shared_ptr<class ColorSampler> CreateSampler() const override;
    virtual std::shared_ptr<class Renderer> CreateRenderer(std::shared_ptr<class Scene> scene, std::shared_ptr<class ColorSampler> sampler) const override;
    virtual int GetSamplesPerPixel() const override;
    virtual bool NotifyNewPixelSample(glm::vec3 inputSampleColor, int sampleIndex) override;
    virtual int GetMaxReflectionBounces() const override;
    virtual int GetMaxRefractionBounces() const override;
    virtual glm::vec2 GetImageOutputResolution() const override;
};
