#pragma once

#include "common/common.h"
#include "common/Acceleration/UniformGrid/Internal/Voxel.h"
#include "common/Scene/Geometry/Simple/Box/Box.h"
#include <unordered_map>

class VoxelGrid : public std::enable_shared_from_this<VoxelGrid>
{
public:
    VoxelGrid(Box inputBox, const glm::ivec3& size, float inputSize);

    void AddNodeToGrid(std::shared_ptr<class AccelerationNode> node);
    bool Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection);
private:
    bool IsInsideGrid(const glm::ivec3& index) const;
    glm::ivec3 GetVoxelForPosition(const glm::vec3& position) const;

    Box boundingBox;
    glm::ivec3 gridSize;
    glm::vec3 voxelSize;
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, Voxel>>> grid;
};