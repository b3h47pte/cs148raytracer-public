#pragma once

#include "common/Acceleration/AccelerationStructure.h"
#include "common/Acceleration/UniformGrid/Internal/VoxelGrid.h"

class UniformGridAcceleration : public AccelerationStructure
{
public:
    UniformGridAcceleration();
    virtual bool Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection) const override;

    void SetSuggestedGridSize(glm::ivec3 input);
private:
    glm::ivec3 gridSize;
    std::unique_ptr<class VoxelGrid> voxelGrid;

    virtual void InternalInitialization() override;
};
