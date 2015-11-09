#pragma once

#include "common/Scene/SceneObject.h"

class Ray : public SceneObject
{
public:
    Ray();
    Ray(glm::vec3 inputPosition, glm::vec3 inputDirection, float inputMaxT = std::numeric_limits<float>::max());

    void SetRayPosition(const glm::vec3& input) { position = glm::vec4(input, 1.f); }
    void SetRayDirection(const glm::vec3& input) { rayDirection = input; } 

    virtual glm::vec4 GetForwardDirection() const override;
    glm::vec3 GetRayDirection() const;

    glm::vec3 GetRayPosition(float t) const;

    float GetMaxT() const;
    void SetMaxT(float input);

    void SetRayMask(uint64_t objectId);
    bool IsObjectMasked(uint64_t objectId);

    glm::vec3 RefractRay(const glm::vec3& normal, float n1, float& n2) const;
private:
    glm::vec3 rayDirection;
    float maxT;

    std::unordered_map<uint64_t, bool> traceMask;
};