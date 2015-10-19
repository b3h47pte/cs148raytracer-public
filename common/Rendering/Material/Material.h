#pragma once

#include "common/common.h"

class Material: public std::enable_shared_from_this<Material>
{
public:
    virtual ~Material();

    virtual glm::vec3 ComputeBRDF(glm::vec3 intersectionPoint, const class Light& relevantLight, const class Ray& toLightRay, const class PrimitiveBase& hitPrimitive) const = 0;
};