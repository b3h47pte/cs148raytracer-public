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

    GenericPhotonMapGeneration(causticMap, causticPhotonNumber, [](const class MeshObject& mesh) {
        const Material* mat = mesh.GetMaterial();
        if (!mat) {
            return false;
        }
        return mat->HasSpecularReflection();
    });
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