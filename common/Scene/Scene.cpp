#include "common/Scene/Scene.h"
#include "common/Scene/Geometry/Ray/Ray.h"
#include "common/Acceleration/AccelerationCommon.h"

void Scene::GenerateAccelerationData(AccelerationTypes sceneType, AccelerationTypes perObjectType)
{
}


bool Scene::Trace(class Ray* inputRay, IntersectionState* outputIntersection) const
{
    assert(inputRay);

    bool didIntersect = false;

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