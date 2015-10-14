#pragma once

#include "common/common.h"

class SceneObject : public std::enable_shared_from_this<SceneObject>
{
public:
    virtual ~SceneObject();
private:
    class std::shared_ptr<class AccelerationStructure> acceleration;
};