#pragma once

#include "common/common.h"
#include "common/Acceleration/AccelerationCommon.h"

class MeshObject: public std::enable_shared_from_this<MeshObject>, public AccelerationNode
{
public:
    MeshObject();
    MeshObject(std::shared_ptr<class Material> inputMaterial);
    virtual ~MeshObject();
    virtual void Finalize();

    void SetName(const std::string& input);
    std::string GetName() const { return meshName; }
    void AddPrimitive(std::shared_ptr<class PrimitiveBase> newPrimitive);
    virtual void CreateAccelerationData(AccelerationTypes perObjectType);

    virtual Box GetBoundingBox() const override
    {
        return boundingBox;
    }

    virtual void SetMaterial(std::shared_ptr<class Material> inputMaterial);
    virtual const class Material* GetMaterial() const;

    virtual bool Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection) const override;

    friend class SceneObject;
protected:
    std::vector<std::shared_ptr<class PrimitiveBase>> elements;
    Box boundingBox;

    class std::shared_ptr<class AccelerationStructure> acceleration;

private:
    std::shared_ptr<class Material> storedMaterial;
    std::string meshName;
};
