#include "common/Rendering/Renderer/Photon/PhotonMappingRenderer.h"
#include "common/Scene/Scene.h"
#include "common/Sampling/ColorSampler.h"
#include "common/Scene/Lights/Light.h"
#include "common/Scene/Geometry/Primitives/Primitive.h"
#include "common/Scene/Geometry/Mesh/MeshObject.h"
#include "common/Rendering/Material/Material.h"
#include "common/Intersection/IntersectionState.h"
#include "common/Scene/SceneObject.h"
#include "common/Scene/Geometry/Mesh/MeshObject.h"
#include "common/Rendering/Material/Material.h"

#define VISUALIZE_PHOTON_MAPPING 1

PhotonMappingRenderer::PhotonMappingRenderer(std::shared_ptr<class Scene> scene, std::shared_ptr<class ColorSampler> sampler):
    Renderer(scene, sampler), diffusePhotonNumber(1000), causticPhotonNumber(1000)
{
}

void PhotonMappingRenderer::InitializeRenderer()
{
    // Generate Photon Maps
    GenericPhotonMapGeneration(diffuseMap, diffusePhotonNumber, [](const class MeshObject& mesh) {
        const Material* mat = mesh.GetMaterial();
        if (!mat) {
            return false;
        }
        return mat->HasDiffuseReflection();
    });

    // Todo if you have time: generate the photon caustic map
}

void PhotonMappingRenderer::GenericPhotonMapGeneration(PhotonKdtree& photonMap, int totalPhotons, std::function<bool(const class MeshObject&)> objectFilter)
{
    std::vector<const MeshObject*> relevantMeshObjects;
    Box relevantMeshBoundingBox;

    size_t totalObjects = storedScene->GetTotalObjects();
    for (size_t i = 0; i < totalObjects; ++i) {
        const SceneObject& object = storedScene->GetSceneObject(i);
        for (int j = 0; j < object.GetTotalMeshObjects(); ++j) {
            const MeshObject* meshObject = object.GetMeshObject(j);
            if (!meshObject || !objectFilter(*meshObject)) {
                continue;
            }
            
            Box meshWorldSpaceBoundingBox = meshObject->GetBoundingBox().Transform(object.GetObjectToWorldMatrix());
            relevantMeshBoundingBox.IncludeBox(meshWorldSpaceBoundingBox);
            relevantMeshObjects.push_back(meshObject);
        }
    }

    // Todo if you have time: Implement projection mapping to make your photon mapper more efficient.

    float totalLightIntensity = 0.f;
    size_t totalLights = storedScene->GetTotalLights();
    for (size_t i = 0; i < totalLights; ++i) {
        const Light* currentLight = storedScene->GetLightObject(i);
        if (!currentLight) {
            continue;
        }
        totalLightIntensity = glm::length(currentLight->GetLightColor());
    }

    // Shoot photons -- number of photons for light is proportional to the light's intensity relative to the total light intensity of the scene.
    for (size_t i = 0; i < totalLights; ++i) {
        const Light* currentLight = storedScene->GetLightObject(i);
        if (!currentLight) {
            continue;
        }

        const float proportion = glm::length(currentLight->GetLightColor()) / totalLightIntensity;
        const int totalPhotonsForLight = static_cast<const int>(proportion * totalPhotons);
        const glm::vec3 photonIntensity = currentLight->GetLightColor() / static_cast<float>(totalPhotonsForLight);
        for (int j = 0; j < totalPhotonsForLight; ++j) {
            Ray photonRay;
            std::vector<char> path;
            path.push_back('L');
            currentLight->GenerateRandomPhotonRay(photonRay);
            TracePhoton(photonRay, photonIntensity, path);
        }
    }
}

void PhotonMappingRenderer::TracePhoton(const class Ray& photonRay, glm::vec3 lightIntensity, std::vector<char>& path)
{
}

glm::vec3 PhotonMappingRenderer::ComputeSampleColor(const struct IntersectionState& intersection, const class Ray& fromCameraRay) const
{
#if VISUALIZE_PHOTON_MAPPING
    return glm::vec3();
#else
    return glm::vec3();
#endif
}

void PhotonMappingRenderer::SetNumberOfDiffusePhotons(int diffuse)
{
    diffusePhotonNumber = diffuse;
}

void PhotonMappingRenderer::SetNumberOfCasuticPhotons(int caustic)
{
    causticPhotonNumber = caustic;
}