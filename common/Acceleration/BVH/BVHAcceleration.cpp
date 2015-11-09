#include "common/Acceleration/BVH/BVHAcceleration.h"
#include "common/Acceleration/BVH/Internal/BVHNode.h"
#include "common/Scene/Geometry/Ray/Ray.h"
#include "common/Intersection/IntersectionState.h"

BVHAcceleration::BVHAcceleration():
    maximumChildren(2), nodesOnLeaves(2)
{
}

bool BVHAcceleration::Trace(const SceneObject* parentObject, Ray* inputRay, IntersectionState* outputIntersection) const
{
    return rootNode->Trace(parentObject, inputRay, outputIntersection);
}

void BVHAcceleration::InternalInitialization()
{
#if !DISABLE_ACCELERATION_CREATION_TIMER
    DIAGNOSTICS_TIMER(timer, "BVH Creation Time");
#endif
    // maximum children shouldn't be less than nodes on leaves...
    if (maximumChildren < nodesOnLeaves) {
        std::cerr << "WARNING: Maximum children is less than nodes on leaves. Setting it equal." << std::endl;
        maximumChildren = nodesOnLeaves;
    }

    rootNode = std::make_shared<BVHNode>(nodes, maximumChildren, nodesOnLeaves);
}

void BVHAcceleration::SetMaximumChildren(int input)
{
    maximumChildren = input;
}

void BVHAcceleration::SetNodesOnLeaves(int input)
{
    nodesOnLeaves = input;
}