#include "common/Acceleration/UniformGrid/UniformGridAcceleration.h"
#include "common/Acceleration/UniformGrid/Internal/VoxelGrid.h"
#include "common/Scene/Geometry/Ray/Ray.h"

UniformGridAcceleration::UniformGridAcceleration():
    gridSize(10, 10, 10), voxelGrid(nullptr)
{
}

bool UniformGridAcceleration::Trace(const SceneObject* parentObject, Ray* inputRay, IntersectionState* outputIntersection) const
{
    assert(voxelGrid);
    return voxelGrid->Trace(parentObject, inputRay, outputIntersection);
}

void UniformGridAcceleration::InternalInitialization()
{
    Box gridBoundingBox;
    DIAGNOSTICS_TIMER(timer, "Uniform Grid Creation Time");
    for (size_t i = 0; i < nodes.size(); ++i) {
        gridBoundingBox.IncludeBox(nodes[i]->GetBoundingBox());
    }

    // Change grid size so that we are using square voxels.
    glm::vec3 gridDiagonal = gridBoundingBox.maxVertex - gridBoundingBox.minVertex;
    float smallestVoxelSide = std::numeric_limits<float>::max();
    for (int i = 0; i < 3; ++i) {
        smallestVoxelSide = std::min(smallestVoxelSide, gridDiagonal[i] / static_cast<float>(gridSize[i]));
    }
    
    const glm::ivec3 suggestedGridSize = gridSize;
    for (int i = 0; i < 3; ++i) {
        gridSize[i] = static_cast<int>(std::ceil(gridDiagonal[i] / smallestVoxelSide)) + 1;
    }
    DIAGNOSTICS_LOG("Final Uniform Grid Size: " + glm::to_string(gridSize) + " from Suggested: " + glm::to_string(suggestedGridSize));
    voxelGrid = make_unique<VoxelGrid>(gridBoundingBox, gridSize, smallestVoxelSide);

    for (size_t i = 0; i < nodes.size(); ++i) {
        voxelGrid->AddNodeToGrid(nodes[i]);
    }
}

void UniformGridAcceleration::SetSuggestedGridSize(glm::ivec3 input)
{
    gridSize = input;
}
