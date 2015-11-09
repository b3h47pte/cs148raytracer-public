#pragma once

#include "common/common.h"
#include "common/Scene/Geometry/Simple/Box/Box.h"
#include <stdint.h>
#include <atomic>

class AccelerationNode
{
public:
    AccelerationNode();

    virtual Box GetBoundingBox() const = 0;
    virtual bool Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection) const = 0;
    virtual uint64_t GetUniqueId() const { return uniqueId; }
    virtual std::string GetHumanIdentifier() const { return ""; }
private:
    static std::atomic<uint64_t> globalIdCount;
    uint64_t uniqueId;
};