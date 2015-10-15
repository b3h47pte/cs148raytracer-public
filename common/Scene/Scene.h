#pragma once

#include "common/common.h"
#include "common/Scene/Intersection/IntersectionState.h"

enum class AccelerationTypes;

class Scene : public std::enable_shared_from_this<Scene>
{
public:
    void GenerateAccelerationData(AccelerationTypes sceneType, AccelerationTypes perObjectType);

    // if outputIntersection is NULL, this merely checks whether or not the inputRay hits something.
    // if outputIntersection is NOT NULL, then this will check whether or not the inputRay hits something,
    //      and if it does, it will store that information and perform reflection/refraction and keep going.
    bool Trace(class Ray* inputRay, IntersectionState* outputIntersection) const;
private:
    class std::shared_ptr<class AccelerationStructure> acceleration;
};