#include "common/Scene/Geometry/Mesh/MeshObject.h"
#include "common/Acceleration/AccelerationCommon.h"
#include "common/Scene/Geometry/Primitives/PrimitiveBase.h"
#include "common/Scene/Geometry/Ray/Ray.h"
#include "common/Scene/SceneObject.h"
#include "common/Intersection/IntersectionState.h"

MeshObject::MeshObject() :
    storedMaterial(nullptr)
{
}

MeshObject::MeshObject(std::shared_ptr<Material> inputMaterial) :
    storedMaterial(std::move(inputMaterial))
{
}

MeshObject::~MeshObject()
{
}

void MeshObject::AddPrimitive(std::shared_ptr<PrimitiveBase> newPrimitive)
{
    elements.emplace_back(std::move(newPrimitive));
}

void MeshObject::Finalize()
{
    boundingBox.Reset();
    for (size_t i = 0; i < elements.size(); ++i) {
        elements[i]->Finalize();
        boundingBox.IncludeBox(elements[i]->GetBoundingBox());
    }
    assert(acceleration);
    acceleration->Initialize(elements);
}

void MeshObject::CreateAccelerationData(AccelerationTypes perObjectType)
{
    acceleration = AccelerationGenerator::CreateStructureFromType(perObjectType);
    assert(acceleration);
}

bool MeshObject::Trace(const SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection) const
{
    return acceleration->Trace(parentObject, inputRay, outputIntersection);
}

const Material* MeshObject::GetMaterial() const
{
    return storedMaterial.get();
}

void MeshObject::SetMaterial(std::shared_ptr<Material> inputMaterial)
{
    storedMaterial = std::move(inputMaterial);
}

void MeshObject::SetName(const std::string& input)
{
    meshName = input;
}