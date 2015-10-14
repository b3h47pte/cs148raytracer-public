#pragma once

#include "common/common.h"

class AccelerationStructure
{
public:
    AccelerationStructure();
    virtual ~AccelerationStructure();

    virtual void Initialize(SceneDataPtr inputSceneData);
protected:
    SceneDataPtr sceneData;
};