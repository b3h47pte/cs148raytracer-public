#pragma once

#include "common/Acceleration/AccelerationStructure.h"

class NaiveAcceleration : public AccelerationStructure
{
public:

    // Only implemented for naive acceleration since it's trivial...
    void AddNode(std::shared_ptr<AccelerationNode> node);

    virtual bool Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection) const override;
};