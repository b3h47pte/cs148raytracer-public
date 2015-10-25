#pragma once

#include "common/common.h"
#include "common/Scene/Geometry/Simple/Box/Box.h"

class AccelerationNode
{
public:
    virtual Box GetBoundingBox() = 0;
    virtual bool Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection) const = 0;
};