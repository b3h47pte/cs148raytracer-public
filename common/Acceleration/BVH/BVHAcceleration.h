#pragma once

#include "common/Acceleration/AccelerationStructure.h"

class BVHAcceleration : public AccelerationStructure
{
public:
    BVHAcceleration();
    virtual bool Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection) const override;

    void SetMaximumChildren(int input);
    void SetNodesOnLeaves(int input);

private:
    virtual void InternalInitialization() override;

    int maximumChildren;
    int nodesOnLeaves;

    std::shared_ptr<class BVHNode> rootNode;
};