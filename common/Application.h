#pragma once

#include "common/common.h"

enum class AccelerationTypes;

class Application : public std::enable_shared_from_this<Application>
{
public:
    virtual ~Application() {}
    virtual std::shared_ptr<class Camera> CreateCamera() const = 0;
    virtual std::shared_ptr<class Scene> CreateScene() const = 0;
    virtual std::shared_ptr<class ColorSampler> CreateSampler() const = 0;
    virtual std::shared_ptr<class Renderer> CreateRenderer(std::shared_ptr<class Scene> scene, std::shared_ptr<class ColorSampler> sampler) const = 0;

    // ray tracing properties
    virtual int GetMaxReflectionBounces() const = 0;
    virtual int GetMaxRefractionBounces() const = 0;

    // output
    virtual glm::vec2 GetImageOutputResolution() const;

    // Sampling Properties
    virtual int GetSamplesPerPixel() const;

    // whether or not to continue sampling the scene from the camera.
    virtual bool NotifyNewPixelSample(glm::vec3 inputSampleColor, int sampleIndex) = 0;

    // Postprocessing
    virtual void PerformImagePostprocessing(class ImageWriter& imageWriter);

    virtual std::string GetOutputFilename() const;
private:
};