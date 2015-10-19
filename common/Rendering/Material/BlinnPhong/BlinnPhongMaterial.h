#pragma once

#include "common/Rendering/Material/Material.h"

class BlinnPhongMaterial : public Material
{
public:
    virtual glm::vec3 ComputeBRDF(glm::vec3 intersectionPoint, const class Light& relevantLight, const class Ray& toLightRay, const class PrimitiveBase& hitPrimitive) const;
};