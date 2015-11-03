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
#if !DISABLE_ACCELERATION_CREATION_TIMER
    DIAGNOSTICS_TIMER(timer, "Uniform Grid Creation Time");
#endif
    for (size_t i = 0; i < nodes.size(); ++i) {
        gridBoundingBox.IncludeBox(nodes[i]->GetBoundingBox());
    }

    glm::vec3 gridDiagonal = gridBoundingBox.maxVertex - gridBoundingBox.minVertex;
    for (int i = 0; i < 3; ++i) {
        if (std::abs(gridDiagonal[i]) < LARGE_EPSILON) {
            // Arbitrary Expansion
            gridBoundingBox.maxVertex[i] += 0.1f;
            gridBoundingBox.minVertex[i] -= 0.1f;
        }
    }
    gridDiagonal = gridBoundingBox.maxVertex - gridBoundingBox.minVertex;

    glm::vec3 voxelSize = gridDiagonal / glm::vec3(gridSize);
    voxelGrid = make_unique<VoxelGrid>(gridBoundingBox, gridSize, voxelSize);

    for (size_t i = 0; i < nodes.size(); ++i) {
        voxelGrid->AddNodeToGrid(nodes[i]);
    }
}

void UniformGridAcceleration::SetSuggestedGridSize(glm::ivec3 input)
{
    gridSize = input;
}
