#pragma once

#include "common/common.h"

class Material: public std::enable_shared_from_this<Material>
{
public:
    virtual ~Material();

    virtual glm::vec3 ComputeBRDF(const struct IntersectionState& IntersectionState, const class Light& relevantLight, const class Ray& toLightRay, const class Ray& fromCameraRay) const = 0;
    virtual std::shared_ptr<Material> Clone() const = 0;
    virtual void LoadMaterialFromAssimp(std::shared_ptr<struct aiMaterial> assimpMaterial) {}
};