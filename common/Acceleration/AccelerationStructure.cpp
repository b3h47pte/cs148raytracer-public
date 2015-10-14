#include "common/Acceleration/AccelerationStructure.h"
#include "common/Scene/SceneObject.h"

AccelerationStructure::AccelerationStructure()
{
}

AccelerationStructure::~AccelerationStructure()
{
}

void AccelerationStructure::Initialize(SceneDataPtr inputSceneData)
{
    sceneData = std::move(inputSceneData);
}