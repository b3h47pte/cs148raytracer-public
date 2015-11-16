#include "common/Acceleration/UniformGrid/Internal/VoxelGrid.h"
#include "common/Acceleration/AccelerationNode.h"
#include "common/Intersection/IntersectionState.h"

#define DEBUG_VOXEL_GRID 0

VoxelGrid::VoxelGrid(Box inputBox, const glm::ivec3& size, const glm::vec3& inputSize):
    boundingBox(inputBox), gridSize(size), voxelSize(inputSize)
{
    const float currentVolume = boundingBox.Volume();
    boundingBox = boundingBox.Expand(0.001f);
    const float newVolume = boundingBox.Volume();
    voxelSize *= newVolume / currentVolume;
}

void VoxelGrid::AddNodeToGrid(std::shared_ptr<AccelerationNode> node)
{
    const Box inputBox = node->GetBoundingBox();
    // Find all nodes that overlap.
    glm::ivec3 minNode = GetVoxelForPosition(inputBox.minVertex);
    glm::ivec3 maxNode = GetVoxelForPosition(inputBox.maxVertex);

#if DEBUG_VOXEL_GRID
    std::cout << "Add: " << node->GetHumanIdentifier() << std::endl;
    std::cout << "Min: " << glm::to_string(minNode) << " " << glm::to_string(inputBox.minVertex) << " " << glm::to_string(boundingBox.minVertex) << std::endl;
    std::cout << "Max: " << glm::to_string(maxNode) << " " << glm::to_string(inputBox.maxVertex) << " " << glm::to_string(boundingBox.maxVertex) << std::endl;
#endif

    for (int i = minNode[0]; i <= maxNode[0]; ++i) {
        for (int j = minNode[1]; j <= maxNode[1]; ++j) {
            for (int k = minNode[2]; k <= maxNode[2]; ++k) {
                grid[i][j][k].AddNode(node);
            }
        }
    }
}

glm::ivec3 VoxelGrid::GetVoxelForPosition(const glm::vec3& position, bool clamp) const
{
    glm::vec3 positionDiff = (position - boundingBox.minVertex) / voxelSize;
    glm::ivec3 iDiff(positionDiff);
    if (clamp) {
        for (int i = 0; i < 3; ++i) {
            iDiff[i] = std::min(std::max(0, iDiff[i]), gridSize[i] - 1);
        }
    }
    return iDiff;
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
    glm::ivec3 currentVoxelIndex = GetVoxelForPosition(rayPos, false);
#if DEBUG_VOXEL_GRID
    std::cout << "Initial Voxel Position: " << glm::to_string(currentVoxelIndex) << " for " << glm::to_string(rayPos) << " going " << glm::to_string(rayDir) << std::endl;
#endif
    // If we aren't currently within the grid, move the ray position such that we are.
    if (!IsInsideGrid(currentVoxelIndex)) {
        IntersectionState tempState;
        if (!boundingBox.Trace(parentObject, inputRay, &tempState)) {
            return false;
        }
        const float dt = tempState.intersectionT + SMALL_EPSILON;
        currentVoxelIndex = GetVoxelForPosition(rayPos + rayDir * dt);
    }
       
#if DEBUG_VOXEL_GRID
    std::cout << "Final Voxel Position: " << glm::to_string(currentVoxelIndex) << " for " << glm::to_string(rayPos) << " going " << glm::to_string(rayDir) << std::endl;
    std::cout << "Scene Bounding: " << glm::to_string(boundingBox.minVertex) << " " << glm::to_string(boundingBox.maxVertex) << std::endl;
    std::cout << "Voxel Size: " << glm::to_string(voxelSize) << std::endl;
#endif
    while (IsInsideGrid(currentVoxelIndex)) {
#if DEBUG_VOXEL_GRID
        std::cout << "Trace Voxel: " << glm::to_string(currentVoxelIndex) << std::endl;
#endif
        IntersectionState tempIntersection;
        tempIntersection.TestAndCopyLimits(outputIntersection);
        bool hitVoxel = grid[currentVoxelIndex[0]][currentVoxelIndex[1]][currentVoxelIndex[2]].Trace(parentObject, inputRay, &tempIntersection);
            
        // Need to verify that the hit position is within the voxel -- otherwise we're looking too far ahead.
        const glm::vec3 hitPosition = rayPos + rayDir * tempIntersection.intersectionT;
#if DEBUG_VOXEL_GRID
        std::cout << "  -- hit position " << glm::to_string(hitPosition) << " " << glm::to_string(GetVoxelForPosition(hitPosition)) << std::endl;
#endif
        if (hitVoxel && GetVoxelForPosition(hitPosition) == currentVoxelIndex)  {
            if (outputIntersection) {
                *outputIntersection = tempIntersection;
            }
#if DEBUG_VOXEL_GRID
            std::cout << " did done hit" << std::endl;
#endif
            return true;
        }

        int minIndex = 0;
        float minTMax = 0.f;
        FindClosestVoxelSide(minIndex, minTMax, currentVoxelIndex, step, rayPos, rayDir);
        assert(minIndex >= 0);
        currentVoxelIndex[minIndex] += step[minIndex];

#if DEBUG_VOXEL_GRID
        std::cout << " -- next voxel: " << glm::to_string(currentVoxelIndex) << " " << minIndex << " " << minTMax << std::endl;
#endif
    }
    return false;
}

void VoxelGrid::FindClosestVoxelSide(int& dim, float& t, const glm::ivec3& currentVoxelIndex, const glm::ivec3& step, const glm::vec3& rayPos, const glm::vec3& rayDir) const
{
    const glm::vec3 index(currentVoxelIndex);
    const glm::vec3 voxelBoxMinCorner = boundingBox.minVertex + index * voxelSize;
    const glm::vec3 voxelBoxMaxCorner = voxelBoxMinCorner + voxelSize;
    float minTMax = std::numeric_limits<float>::max();
    int minIndex = -1;
    for (int i = 0; i < 3; ++i) {
        if (!step[i]) {
            continue;
        }
        const float tMax = (step[i] == 1) ? (voxelBoxMaxCorner[i] - rayPos[i]) / rayDir[i] : (voxelBoxMinCorner[i] - rayPos[i]) / rayDir[i];
#if DEBUG_VOXEL_GRID
        std::cout << "  max min: " << voxelBoxMaxCorner[i] << " " << voxelBoxMinCorner[i] << std::endl;
        std::cout << "  ray travel: " << rayPos[i] << " " << rayDir[i] << std::endl;
#endif
        if (tMax < minTMax && tMax > -SMALL_EPSILON) {
            minTMax = tMax;
            minIndex = i;
        }
    }
    t = minTMax;
    dim = minIndex;
}
