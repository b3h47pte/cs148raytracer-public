#pragma once

#include "common/Scene/Geometry/Primitives/Primitive.h"

class Triangle: public Primitive<3>
{
public:
    Triangle(class MeshObject* inputParent);
    virtual bool Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection) const override;
    virtual glm::vec3 GetPrimitiveNormal() const override;
};