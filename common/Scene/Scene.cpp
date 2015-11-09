#include "common/Scene/Scene.h"
#include "common/Scene/Geometry/Ray/Ray.h"
#include "common/Scene/Geometry/Primitives/PrimitiveBase.h"
#include "common/Scene/Geometry/Mesh/MeshObject.h"
#include "common/Rendering/Material/Material.h"
#include "common/Acceleration/AccelerationCommon.h"

void Scene::GenerateDefaultAccelerationData()
{
    if (!acceleration) {
        GenerateAccelerationData(AccelerationTypes::NONE);
    }

    assert(acceleration);

    for (size_t i = 0; i < sceneObjects.size(); ++i) {
        sceneObjects[i]->CreateDefaultAccelerationData();
    }
}

AccelerationStructure* Scene::GenerateAccelerationData(AccelerationTypes inputType)
{
    acceleration = AccelerationGenerator::CreateStructureFromType(inputType);
    return acceleration.get();
}


bool Scene::Trace(class Ray* inputRay, IntersectionState* outputIntersection) const
{
    assert(inputRay);
    DIAGNOSTICS_STAT(DiagnosticsType::RAYS_CREATED);

    bool didIntersect = acceleration->Trace(nullptr, inputRay, outputIntersection);
    if (outputIntersection != nullptr && didIntersect) {
        const MeshObject* intersectedMesh = outputIntersection->intersectedPrimitive->GetParentMeshObject();
        assert(intersectedMesh);
        const Material* currentMaterial = intersectedMesh->GetMaterial();
        assert(currentMaterial);

        const glm::vec3 intersectionPoint = outputIntersection->intersectionRay.GetRayPosition(outputIntersection->intersectionT);
        const float NdR = glm::dot(inputRay->GetRayDirection(), outputIntersection->ComputeNormal());
        // send out reflection ray.
        if (currentMaterial->IsReflective() && outputIntersection->remainingReflectionBounces > 0) {
            outputIntersection->reflectionIntersection = std::make_shared<IntersectionState>(outputIntersection->remainingReflectionBounces - 1, outputIntersection->remainingRefractionBounces);

            const glm::vec3 normal = (NdR > SMALL_EPSILON) ? -1.f * outputIntersection->ComputeNormal(): outputIntersection->ComputeNormal();
            const glm::vec3 reflectionDir = glm::reflect(inputRay->GetRayDirection(), normal);
            Ray reflectionRay(intersectionPoint + reflectionDir * LARGE_EPSILON, reflectionDir);
            Trace(&reflectionRay, outputIntersection->reflectionIntersection.get());
        }

        // send out refraction ray.
        if (currentMaterial->IsTransmissive() && outputIntersection->remainingRefractionBounces > 0) {
            outputIntersection->refractionIntersection = std::make_shared<IntersectionState>(outputIntersection->remainingReflectionBounces, outputIntersection->remainingRefractionBounces - 1);

            // If we're going into the mesh, set the target IOR to be the IOR of the mesh.
            float targetIOR = (NdR < SMALL_EPSILON) ? currentMaterial->GetIOR() : 1.f;
            const glm::vec3 refractionDir = inputRay->RefractRay(outputIntersection->ComputeNormal(), outputIntersection->currentIOR, targetIOR);
            
            outputIntersection->refractionIntersection->currentIOR = targetIOR;

            Ray refractionRay(intersectionPoint + refractionDir * LARGE_EPSILON, refractionDir);
            Trace(&refractionRay, outputIntersection->refractionIntersection.get());
        }
    }

    return didIntersect;
}

void Scene::AddSceneObject(std::shared_ptr<SceneObject> object)
{
    if (!object) {
        return;
    }
    sceneObjects.emplace_back(std::move(object));
}

void Scene::AddLight(std::shared_ptr<Light> light)
{
    if (!light) {
        return;
    }
    sceneLights.emplace_back(std::move(light));
}

void Scene::Finalize()
{
    for (size_t i = 0; i < sceneObjects.size(); ++i) {
        sceneObjects[i]->Finalize();
    }
    assert(acceleration);
    acceleration->Initialize(sceneObjects);
}