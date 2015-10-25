#include "common/Scene/Scene.h"
#include "common/Scene/Geometry/Ray/Ray.h"
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

void Scene::GenerateAccelerationData(AccelerationTypes inputType)
{
    acceleration = AccelerationGenerator::CreateStructureFromType(inputType);
}


bool Scene::Trace(class Ray* inputRay, IntersectionState* outputIntersection) const
{
    assert(inputRay);

    bool didIntersect = acceleration->Trace(nullptr, inputRay, outputIntersection);
    if (outputIntersection != nullptr) {
        // send out reflection ray.

        // send out refraction ray.
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