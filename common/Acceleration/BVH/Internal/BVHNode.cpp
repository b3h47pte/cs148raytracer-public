#include "common/Acceleration/BVH/Internal/BVHNode.h"
#include "common/Acceleration/AccelerationNode.h"
#include "common/Intersection/IntersectionState.h"

BVHNode::BVHNode(std::vector<std::shared_ptr<AccelerationNode>>& childObjects, int maximumChildren, int nodesOnLeaves, int splitDim):
    isLeafNode(false)
{
    if (childObjects.size() <= nodesOnLeaves) {
        CreateLeafNode(childObjects);
    } else {
        CreateParentNode(childObjects, maximumChildren, nodesOnLeaves, splitDim);
    }
}

void BVHNode::CreateLeafNode(std::vector<std::shared_ptr<class AccelerationNode>>& childObjects)
{
    isLeafNode = true;
    leafNodes.insert(leafNodes.end(), childObjects.begin(), childObjects.end());
    for (size_t i = 0; i < leafNodes.size(); ++i) {
        boundingBox.IncludeBox(leafNodes[i]->GetBoundingBox());
    }
}

void BVHNode::CreateParentNode(std::vector<std::shared_ptr<class AccelerationNode>>& childObjects, int maximumChildren, int nodesOnLeaves, int splitDim)
{
    // Sort nodes based on their positions using the current dimension.
    std::sort(childObjects.begin(), childObjects.end(), [=](std::shared_ptr<AccelerationNode>& a, std::shared_ptr<AccelerationNode>& b) {
        return (a->GetBoundingBox().Center()[splitDim] < b->GetBoundingBox().Center()[splitDim]);
    });

    const int nextDim = (splitDim + 1) % 3;

    // Now split this up into the children nodes. At this point we know that the number of nodes left is definitely larger than nodesOnLeaves which is greater than or equal to maximumChildren.
    // Thus we are guaranteed to have nodesPerChild be at least one.
    const int nodesPerChild = static_cast<int>(childObjects.size()) / maximumChildren;
    assert(nodesPerChild >= 1);

    for (int i = 0; i < maximumChildren; ++i) {
        const int startIndex = i * nodesPerChild;
        const int elementsToUse = (i == maximumChildren - 1) ? static_cast<int>(childObjects.size()) - startIndex : nodesPerChild;

        std::vector<std::shared_ptr<AccelerationNode>> subnodes;
        subnodes.insert(subnodes.end(), childObjects.begin() + startIndex, childObjects.begin() + startIndex + elementsToUse);

        std::shared_ptr<BVHNode> childNode = std::make_shared<BVHNode>(subnodes, maximumChildren, nodesOnLeaves, nextDim);
        childBVHNodes.push_back(childNode);
        boundingBox.IncludeBox(childNode->boundingBox);
    }
}

bool BVHNode::Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection) const
{
    if (!boundingBox.Trace(parentObject, inputRay, nullptr)) {
        return false;
    }

    bool hitObject = false;

    // Merge these two branches....
    if (isLeafNode) {
        for (size_t i = 0; i < leafNodes.size(); ++i) {
            IntersectionState temporaryIntersection;
            hitObject |= leafNodes[i]->Trace(parentObject, inputRay, &temporaryIntersection);
            MergeTemporaryIntersectionToOutput(temporaryIntersection, outputIntersection);
        }
    } else {
        for (size_t i = 0; i < childBVHNodes.size(); ++i) {
            IntersectionState temporaryIntersection;
            hitObject |= childBVHNodes[i]->Trace(parentObject, inputRay, &temporaryIntersection);
            MergeTemporaryIntersectionToOutput(temporaryIntersection, outputIntersection);
        }
    }
    
    return hitObject;
}

void BVHNode::MergeTemporaryIntersectionToOutput(const IntersectionState& temporaryIntersection, IntersectionState* outputIntersection) const
{
    if (!temporaryIntersection.hasIntersection || !outputIntersection) {
        return;
    }

    if (outputIntersection->hasIntersection && temporaryIntersection.intersectionT - outputIntersection->intersectionT > SMALL_EPSILON) {
        return;
    }

    *outputIntersection = temporaryIntersection;
}