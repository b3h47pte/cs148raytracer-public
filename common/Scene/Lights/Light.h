#pragma once

#include "common/Scene/SceneObject.h"
#include "common/Scene/Geometry/Ray/Ray.h"

class Light : public SceneObject
{
public:
    virtual void ComputeSampleRays(std::vector<Ray>& output, glm::vec3 origin) const = 0;
    virtual float ComputeLightAttenuation(glm::vec3 origin) const = 0;
};