#pragma once

#include "common/common.h"
#include "common/Scene/Geometry/Ray/Ray.h"

struct IntersectionState
{
    IntersectionState() :
        reflectionIntersection(nullptr), remainingReflectionBounces(0), refractionIntersection(nullptr), remainingRefractionBounces(0), intersectionT(std::numeric_limits<float>::max()), hasIntersection(false), currentIOR(1.f)
    {
    }

    IntersectionState(int reflectionBounces, int refractionBounces) :
        reflectionIntersection(nullptr), remainingReflectionBounces(reflectionBounces), refractionIntersection(nullptr), remainingRefractionBounces(refractionBounces), intersectionT(std::numeric_limits<float>::max()), hasIntersection(false), currentIOR(1.f)
    {
    }

    void TestAndCopyLimits(IntersectionState* state)
    {
        if (!state) {
            return;
        }
        remainingReflectionBounces = state->remainingReflectionBounces;
        remainingRefractionBounces = state->remainingRefractionBounces;
        intersectionT = state->intersectionT;
        currentIOR = state->currentIOR;
    }

    std::shared_ptr<struct IntersectionState> reflectionIntersection;
    int remainingReflectionBounces;

    std::shared_ptr<struct IntersectionState> refractionIntersection;
    int remainingRefractionBounces;

    const class PrimitiveBase* intersectedPrimitive;
    const class SceneObject* primitiveParent;
    Ray intersectionRay;
    float intersectionT;
    bool hasIntersection;
    float currentIOR;

    // One for each vertex
    std::vector<float> primitiveIntersectionWeights;

    // Utility Functions
    glm::vec3 ComputeNormal() const;
    glm::vec2 ComputeUV() const;
};