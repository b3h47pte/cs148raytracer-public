#pragma once

#include "common/common.h"

enum class AccelerationTypes;

class Scene : public std::enable_shared_from_this<Scene>
{
public:
    void GenerateAccelerationData(AccelerationTypes sceneType, AccelerationTypes perObjectType);
private:
    class std::shared_ptr<class AccelerationStructure> acceleration;
};