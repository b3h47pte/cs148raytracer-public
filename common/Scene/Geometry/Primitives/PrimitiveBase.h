#pragma once

#include "common/common.h"
#include "common/Acceleration/AccelerationNode.h"

class PrimitiveBase: public AccelerationNode
{
public:
    virtual const class MeshObject* GetParentMeshObject() const = 0;
    virtual void SetVertexPosition(int index, glm::vec3 position) = 0;
    virtual void SetVertexNormal(int index, glm::vec3 normal) = 0;
    virtual void SetVertexUV(int index, glm::vec2 uv) = 0;
    virtual void SetVertexTangentBitangent(int index, glm::vec3 tangent, glm::vec3 bitangent) = 0;
    virtual int GetTotalVertices() const = 0;
    virtual void Finalize() = 0;

    virtual bool HasVertexNormals() const = 0;
    virtual bool HasNormalMap() const = 0;
    virtual glm::vec3 GetVertexNormal(int index) const = 0;
    virtual glm::vec3 GetVertexNormalMap(glm::vec2 uv, const glm::vec3& worldTangent, const glm::vec3& worldBitangent, const glm::vec3& worldNormal) const = 0;
    virtual glm::vec3 GetPrimitiveNormal() const = 0;
    virtual glm::vec2 GetVertexUV(int index) const = 0;
    virtual glm::vec3 GetVertexTangent(int index) const = 0;
    virtual glm::vec3 GetVertexBitangent(int index) const = 0;
};