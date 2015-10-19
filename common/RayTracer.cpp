#include "common/RayTracer.h"
#include "common/Application.h"
#include "common/Scene/Scene.h"
#include "common/Scene/Camera/Camera.h"
#include "common/Scene/Geometry/Ray/Ray.h"
#include "common/Intersection/IntersectionState.h"
#include "common/Sampling/Sampler.h"
#include "common/Output/ImageWriter.h"
#include "common/Rendering/Renderer.h"

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
    std::shared_ptr<Renderer> currentRenderer = storedApplication->CreateRenderer(currentScene, currentSampler);
    assert(currentScene && currentCamera && currentSampler && currentRenderer);

    currentRenderer->InitializeRenderer();

    // Scene preprocessing -- generate acceleration structures, etc.
    // After this call, we are guaranteed that the "acceleration" member of the scene and all scene objects within the scene will be non-NULL.
    currentScene->GenerateAccelerationData(storedApplication->GetSceneAccelerationType(), storedApplication->GetPerObjectAccelerationType());
    currentScene->Finalize();

    // Prepare for Output
    const glm::vec2 currentResolution = storedApplication->GetImageOutputResolution();
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

                glm::vec2 normalizedCoordinates(static_cast<float>(c) + sampleOffset.x, static_cast<float>(r)+sampleOffset.y);
                normalizedCoordinates /= currentResolution;

                // Construct ray, send it out into the scene and see what we hit.
                std::shared_ptr<Ray> cameraRay = currentCamera->GenerateRayForNormalizedCoordinates(normalizedCoordinates);
                assert(cameraRay);

                IntersectionState rayIntersection(storedApplication->GetMaxReflectionBounces(), storedApplication->GetMaxRefractionBounces());
                bool didHitScene = currentScene->Trace(cameraRay.get(), &rayIntersection);

                // Use the intersection data to compute the BRDF response.
                glm::vec3 sampleColor;
                if (didHitScene) {
                    sampleColor = currentRenderer->ComputeSampleColor(rayIntersection);
                    sampleColor = glm::vec3(1.f, 0.f, 0.f);
                }

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

    // Now copy whatever is in the HDR data and store it in the bitmap that we will save (aka everything will get clamped to be [0.0, 1.0]).
    imageWriter.CopyHDRToBitmap();

    // Save image.
    imageWriter.SaveImage();
}