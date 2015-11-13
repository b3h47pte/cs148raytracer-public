#pragma once

#include "common/common.h"
#include "common/Acceleration/UniformGrid/Internal/Voxel.h"
#include "common/Scene/Geometry/Simple/Box/Box.h"
#include <unordered_map>

class VoxelGrid : public std::enable_shared_from_this<VoxelGrid>
{
public:
    VoxelGrid(Box inputBox, const glm::ivec3& size, const glm::vec3& inputSize);

    void AddNodeToGrid(std::shared_ptr<class AccelerationNode> node);
    bool Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection);
private:
    bool IsInsideGrid(const glm::ivec3& index) const;
    glm::ivec3 GetVoxelForPosition(const glm::vec3& position, bool clamp = true) const;
    void FindClosestVoxelSide(int& dim, float& t, const glm::ivec3& currentVoxelIndex, const glm::ivec3& step, const glm::vec3& rayPos, const glm::vec3& rayDir) const;

    Box boundingBox;
    glm::ivec3 gridSize;
    glm::vec3 voxelSize;
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, Voxel>>> grid;
};