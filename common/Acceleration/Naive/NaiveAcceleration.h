#pragma once

#include "common/Acceleration/AccelerationStructure.h"

class NaiveAcceleration : public AccelerationStructure
{
public:
    virtual bool Trace(class Ray* inputRay, struct IntersectionState* outputIntersection) const override;
};