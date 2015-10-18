#include "common/Scene/Scene.h"
#include "common/Scene/Geometry/Ray/Ray.h"
#include "common/Acceleration/AccelerationCommon.h"

void Scene::GenerateAccelerationData(AccelerationTypes sceneType, AccelerationTypes perObjectType)
{
    acceleration = AccelerationGenerator::CreateStructureFromType(sceneType);
    assert(acceleration);
    for (size_t i = 0; i < sceneObjects.size(); ++i) {
        sceneObjects[i]->CreateAccelerationData(perObjectType);
    }
}


bool Scene::Trace(class Ray* inputRay, IntersectionState* outputIntersection) const
{
    assert(inputRay);

    bool didIntersect = acceleration->Trace(inputRay, outputIntersection);
    if (outputIntersection != nullptr) {

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