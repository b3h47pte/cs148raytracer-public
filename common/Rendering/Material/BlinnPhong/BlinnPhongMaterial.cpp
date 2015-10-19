#include "common/Rendering/Material/BlinnPhong/BlinnPhongMaterial.h"

glm::vec3 BlinnPhongMaterial::ComputeBRDF(glm::vec3 intersectionPoint, const class Light& relevantLight, const class Ray& toLightRay, const class PrimitiveBase& hitPrimitive) const
{
    return glm::vec3();
}