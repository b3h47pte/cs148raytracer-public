#pragma once

#include "common/common.h"
#include "common/Scene/Geometry/Simple/Box/Box.h"

class Voxel : public std::enable_shared_from_this<Voxel>
{
public:
    Voxel();
    ~Voxel();
    void AddNode(std::shared_ptr<class AccelerationNode> input);
    bool Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection);
private:
    std::unique_ptr<class NaiveAcceleration> nodeList;
};