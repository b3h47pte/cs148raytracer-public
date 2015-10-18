#pragma once

#include "common/Scene/Geometry/Primitives/Primitive.h"

class Triangle: public Primitive<3>
{
public:
    virtual bool Trace(class Ray* inputRay, struct IntersectionState* outputIntersection) const override;
};