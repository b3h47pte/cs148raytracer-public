#pragma once

#include "common/Acceleration/AccelerationStructure.h"

class NaiveAcceleration : public AccelerationStructure
{
public:
    virtual bool Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection) const override;
};