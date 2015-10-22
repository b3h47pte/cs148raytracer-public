#pragma once

#include "common/common.h"
#include "common/Scene/Geometry/Ray/Ray.h"

struct IntersectionState
{
    IntersectionState() :
        remainingReflectionBounces(0), remainingRefractionBounces(0), hasIntersection(false)
    {
    }

    IntersectionState(int reflectionBounces, int refractionBounces) :
        remainingReflectionBounces(reflectionBounces), remainingRefractionBounces(refractionBounces), hasIntersection(false)
    {
    }

    void TestAndCopyLimits(IntersectionState* state)
    {
        if (!state) {
            return;
        }
        remainingReflectionBounces = state->remainingReflectionBounces;
        remainingRefractionBounces = state->remainingRefractionBounces;
    }

    int remainingReflectionBounces;
    int remainingRefractionBounces;

    const class PrimitiveBase* intersectedPrimitive;
    const class SceneObject* primitiveParent;
    Ray intersectionRay;
    float intersectionT;
    bool hasIntersection;

    // One for each vertex
    std::vector<float> primitiveIntersectionWeights;

    // Utility Functions
    glm::vec3 ComputeNormal() const;
};