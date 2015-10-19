#pragma once

#include "common/Scene/Geometry/Primitives/PrimitiveBase.h"
#include "common/Scene/SceneObject.h"

template<int N>
class Primitive : public PrimitiveBase, public SceneObject
{
public:
    Primitive(class MeshObject* inputParent):
        parentMesh(inputParent)
    {
    }

    virtual ~Primitive()
    {
    }

    virtual void SetVertexPosition(int index, glm::vec3 position)
    {
        assert(index >= 0 && index < N);
        positions[index] = position;
    }

    virtual void SetVertexNormal(int index, glm::vec3 normal)
    {
        assert(index >= 0 && index < N);
        normals[index] = normal;
    }

    virtual void SetVertexUV(int index, glm::vec2 uv)
    {
        assert(index >= 0 && index < N);
        uvs[index] = uv;
    }

    virtual int GetTotalVertices() const
    {
        return N;
    }

    virtual void Finalize()
    {
        UpdateBoundingBox();
    }

    virtual Box GetBoundingBox() override
    {
        return boundingBox;
    }

    virtual const class MeshObject* GetParentMeshObject() const override
    {
        return parentMesh;
    }
protected:
    std::array<glm::vec3, N> positions;
    std::array<glm::vec3, N> normals;
    std::array<glm::vec2, N> uvs;

    void UpdateBoundingBox()
    {
        boundingBox.Reset();
        for (int i = 0; i < N; ++i) {
            boundingBox.maxVertex = glm::max(boundingBox.maxVertex, positions[i]);
            boundingBox.minVertex = glm::min(boundingBox.minVertex, positions[i]);
        }
    }
    Box boundingBox;
private:
    const class MeshObject* parentMesh;
};