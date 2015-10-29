#pragma once

#include "common/common.h"

class Box
{
public:
    Box();
    Box(glm::vec3 minV, glm::vec3 maxV);

    void Reset();
    void IncludeBox(const Box& box);
    glm::vec3 Center() const;
    float Volume() const;

    bool Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection) const;
    
    Box Expand(float delta) const;
    Box Transform(glm::mat4 transformation) const;
    void Corners(std::vector<glm::vec3>& corners) const;

    glm::vec3 minVertex;
    glm::vec3 maxVertex;
};