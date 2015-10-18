#pragma once

#include "common/common.h"

class MeshObject: public std::enable_shared_from_this<MeshObject>
{
public:
    virtual ~MeshObject();
    virtual void Finalize();

    void AddPrimitive(std::shared_ptr<class PrimitiveBase> newPrimitive);
protected:
    std::vector<std::shared_ptr<class PrimitiveBase>> elements;

    class std::shared_ptr<class AccelerationStructure> acceleration;
};