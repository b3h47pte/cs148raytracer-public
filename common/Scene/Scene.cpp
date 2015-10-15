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