#pragma once

#include "common/common.h"
#include "common/Scene/Geometry/Simple/Box/Box.h"

class BVHNode : public std::enable_shared_from_this <BVHNode>
{
public:
    BVHNode(std::vector<std::shared_ptr<class AccelerationNode>>& childObjects, int maximumChildren, int nodesOnLeaves, int splitDim = 0);
    bool Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection) const;
private:
    void CreateLeafNode(std::vector<std::shared_ptr<class AccelerationNode>>& childObjects);
    void CreateParentNode(std::vector<std::shared_ptr<class AccelerationNode>>& childObjects, int maximumChildren, int nodesOnLeaves, int splitDim);
    std::string PrintContents() const;


    std::vector<std::shared_ptr<BVHNode>> childBVHNodes;
    std::vector<std::shared_ptr<class AccelerationNode>> leafNodes;
    bool isLeafNode;
    Box boundingBox;
};