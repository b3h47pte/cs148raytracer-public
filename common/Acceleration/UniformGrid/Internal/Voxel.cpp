#include "common/Acceleration/UniformGrid/Internal/Voxel.h"
#include "common/Acceleration/AccelerationNode.h"
#include "common/Acceleration/Naive/NaiveAcceleration.h"

Voxel::Voxel()
{
    nodeList = make_unique<NaiveAcceleration>();
}

Voxel::~Voxel()
{
}

void Voxel::AddNode(std::shared_ptr<AccelerationNode> input)
{
    nodeList->AddNode(std::move(input));
}

bool Voxel::Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection)
{
    return nodeList->Trace(parentObject, inputRay, outputIntersection);
}