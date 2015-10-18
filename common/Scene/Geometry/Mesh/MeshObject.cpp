#include "common/Scene/Geometry/Mesh/MeshObject.h"

MeshObject::~MeshObject()
{
}

void MeshObject::AddPrimitive(std::shared_ptr<PrimitiveBase> newPrimitive)
{
    elements.emplace_back(std::move(newPrimitive));
}

void MeshObject::Finalize()
{

}