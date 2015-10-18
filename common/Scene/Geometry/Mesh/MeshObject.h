#pragma once

#include "common/common.h"
#include "common/Acceleration/AccelerationCommon.h"

class MeshObject: public std::enable_shared_from_this<MeshObject>, public AccelerationNode
{
public:
    virtual ~MeshObject();
    virtual void Finalize();

    void AddPrimitive(std::shared_ptr<class PrimitiveBase> newPrimitive);
    virtual void CreateAccelerationData(AccelerationTypes perObjectType);

    virtual Box GetBoundingBox()
    {
        return boundingBox;
    }

    virtual bool Trace(class Ray* inputRay, struct IntersectionState* outputIntersection) const override;
protected:
    std::vector<std::shared_ptr<class PrimitiveBase>> elements;
    Box boundingBox;

    class std::shared_ptr<class AccelerationStructure> acceleration;
};