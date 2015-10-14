#include "common/RayTracer.h"
#include "common/Application.h"
#include "common/Scene/Scene.h"
#include "common/Scene/Camera/Camera.h"
#include "common/Sampling/Sampler.h"
#include "common/Output/ImageWriter.h"

RayTracer::RayTracer(std::unique_ptr<class Application> app):
    storedApplication(std::move(app))
{
}

void RayTracer::Run()
{
    // Scene Setup -- Generate the camera and scene.
    std::shared_ptr<Camera> currentCamera = storedApplication->CreateCamera();
    std::shared_ptr<Scene> currentScene = storedApplication->CreateScene();
    std::shared_ptr<Sampler> currentSampler = storedApplication->CreateSampler();
    assert(currentScene && currentCamera);

    // Scene preprocessing -- generate acceleration structures, etc.
    // After this call, we are guaranteed that the "acceleration" member of the scene and all scene objects within the scene will be non-NULL.
    currentScene->GenerateAccelerationData(storedApplication->GetSceneAccelerationType(), storedApplication->GetPerObjectAccelerationType());

    // Prepare for Output
    const glm::vec2 currentResolution = currentCamera->GetResolution();
    ImageWriter imageWriter(storedApplication->GetOutputFilename(), static_cast<int>(currentResolution.x), static_cast<int>(currentResolution.y));

    // Perform forward ray tracing
    const int maxSamplesPerPixel = storedApplication->GetSamplesPerPixel();
    assert(maxSamplesPerPixel >= 1);

    for (int r = 0; r < static_cast<int>(currentResolution.y); ++r) {
        for (int c = 0; c < static_cast<int>(currentResolution.x); ++c) {
            glm::vec3 finalColor;

            glm::vec2 sampleOffset(0.5f, 0.5f);
            int totalSamplesUsed = 0;
            for (int s = 0; s < maxSamplesPerPixel; ++s) {
                if (maxSamplesPerPixel != 1) {
                    sampleOffset = currentSampler->Sample2D(glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f));
                }

                // Construct ray, send it out into the scene and get a color back.
                glm::vec3 sampleColor;
                finalColor += sampleColor;
                ++totalSamplesUsed;

                if (!storedApplication->NotifyNewPixelSample(sampleColor, s)) {
                    break;
                }
            }
            imageWriter.SetPixelColor(finalColor / static_cast<float>(totalSamplesUsed), c, r);
        }
    }

    // Apply post-processing steps (i.e. tone-mapper, etc.).
    storedApplication->PerformImagePostprocessing(imageWriter);

    // Now copy whatever is in the HDR data and store it in the bitmap that we will save (aka everything will get clamped to 1.0).
    imageWriter.CopyHDRToBitmap();

    // Save image.
    imageWriter.SaveImage();
}