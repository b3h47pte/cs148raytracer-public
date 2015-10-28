#include "common/Acceleration/UniformGrid/Internal/VoxelGrid.h"
#include "common/Acceleration/AccelerationNode.h"
#include "common/Intersection/IntersectionState.h"

VoxelGrid::VoxelGrid(Box inputBox, const glm::ivec3& size, float inputSize):
    boundingBox(inputBox), gridSize(size), voxelSize(inputSize)
{
}

void VoxelGrid::AddNodeToGrid(std::shared_ptr<AccelerationNode> node)
{
    const Box inputBox = node->GetBoundingBox();
    // Find all nodes that overlap.
    glm::ivec3 minNode = GetVoxelForPosition(inputBox.minVertex);
    glm::ivec3 maxNode = GetVoxelForPosition(inputBox.maxVertex);

    for (int i = minNode[0]; i <= maxNode[0]; ++i) {
        for (int j = minNode[1]; j <= maxNode[1]; ++j) {
            for (int k = minNode[1]; k <= maxNode[2]; ++k) {
                grid[i][j][k].AddNode(node);
            }
        }
    }
}

glm::ivec3 VoxelGrid::GetVoxelForPosition(const glm::vec3& position) const
{
    glm::vec3 positionDiff = (position - boundingBox.minVertex) / voxelSize;
    return glm::ivec3(positionDiff);
}

bool VoxelGrid::IsInsideGrid(const glm::ivec3& index) const
{
    for (int i = 0; i < 3; ++i) {
        if (index[i] >= gridSize[i] || index[i] < 0) {
            return false;
        }
    }
    return true;
}

bool VoxelGrid::Trace(const SceneObject* parentObject, Ray* inputRay, IntersectionState* outputIntersection)
{
    glm::mat4 spaceTransform(1.f);
    if (parentObject) {
        spaceTransform = parentObject->GetWorldToObjectMatrix();
    }
    glm::vec3 rayPos = glm::vec3(spaceTransform * inputRay->GetPosition());
    const glm::vec3 rayDir = glm::vec3(spaceTransform * inputRay->GetForwardDirection());
    glm::ivec3 step;
    for (int i = 0; i < 3; ++i) {
        if (std::abs(rayDir[i]) < SMALL_EPSILON) {
            step[i] = 0;
        } else {
            step[i] = (rayDir[i] > SMALL_EPSILON) ? 1 : -1;
        }
    }

    // Implementation of "A Fast Voxel Traversal Algorithm for Ray Tracing" by John Amanatides and Andrew Woo
    // Link: http://www.cse.chalmers.se/edu/year/2010/course/TDA361/grid.pdf
    glm::ivec3 currentVoxelIndex = GetVoxelForPosition(rayPos);
    // If we aren't currently within the grid, move the ray position such that we are.
    if (!IsInsideGrid(currentVoxelIndex)) {
        IntersectionState tempState;
        if (!boundingBox.Trace(parentObject, inputRay, &tempState)) {
            return false;
        }
        rayPos += rayDir * (tempState.intersectionT + SMALL_EPSILON);
        currentVoxelIndex = GetVoxelForPosition(rayPos);
    }
    while (IsInsideGrid(currentVoxelIndex)) {
        try {
            bool hitVoxel = grid.at(currentVoxelIndex[0]).at(currentVoxelIndex[1]).at(currentVoxelIndex[2]).Trace(parentObject, inputRay, outputIntersection);
            if (hitVoxel)  {
                return true;
            }
        } catch (...) {}
        const glm::vec3 index(currentVoxelIndex);
        const glm::vec3 voxelBoxMaxCorner = boundingBox.minVertex + (index + 1.f) * voxelSize;
        float minTMax = std::numeric_limits<float>::max();
        int minIndex = -1;
        for (int i = 0; i < 3; ++i) {
            if (!step[i]) {
                continue;
            }
            const float tMax = (voxelBoxMaxCorner[i] - rayPos[i]) / rayDir[i];
            if (tMax < minTMax) {
                minTMax = tMax;
                minIndex = i;
            }
        }
        assert(minIndex >= 0);
        currentVoxelIndex[minIndex] += step[minIndex];
    }
    return false;
}