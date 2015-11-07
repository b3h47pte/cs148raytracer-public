#pragma once

#include "common/Scene/Geometry/Primitives/PrimitiveBase.h"
#include "common/Scene/Geometry/Mesh/MeshObject.h"
#include "common/Rendering/Material/Material.h"
#include "common/Rendering/Textures/Texture.h"
#include "common/Scene/SceneObject.h"

template<int N>
class Primitive : public PrimitiveBase, public SceneObject
{
public:
    Primitive(class MeshObject* inputParent):
        hasNormals(false), hasUVs(false), hasTangentBitangents(false), parentMesh(inputParent)
    {
    }

    virtual ~Primitive()
    {
    }

    virtual void SetVertexPosition(int index, glm::vec3 position) override
    {
        assert(index >= 0 && index < N);
        positions[index] = position;
    }

    virtual void SetVertexNormal(int index, glm::vec3 normal) override
    {
        // Assume that SetVertexNormal is called IF AND ONLY IF normals are specified.
        assert(index >= 0 && index < N);
        normals[index] = normal;
        hasNormals = true;
    }

    virtual void SetVertexUV(int index, glm::vec2 uv) override
    {
        // Assume that SetVertexUV is called IF AND ONLY IF uv coordinates are specified.
        assert(index >= 0 && index < N);
        uvs[index] = uv;
        hasUVs = true;
    }

    virtual void SetVertexTangentBitangent(int index, glm::vec3 tangent, glm::vec3 bitangent) override
    {
        assert(index >= 0 && index < N);
        tangents[index] = tangent;
        bitangents[index] = bitangent;
        hasTangentBitangents = true;
    }


    virtual int GetTotalVertices() const override
    {
        return N;
    }

    virtual void Finalize() override
    {
        UpdateBoundingBox();
    }

    virtual Box GetBoundingBox() const override
    {
        return boundingBox;
    }

    virtual const class MeshObject* GetParentMeshObject() const override
    {
        return parentMesh;
    }

    virtual bool HasVertexNormals() const override
    {
        return hasNormals;
    }

    virtual glm::vec3 GetVertexNormal(int index) const override
    {
        return normals[index];
    }

    virtual bool HasNormalMap() const override
    {
        const Material* material = parentMesh->GetMaterial();
        if (material && hasUVs) {
            Texture* normalTexture = material->GetTexture("normalTexture");
            if (normalTexture) {
                return true;
            }
        }
        return false;
    }

    virtual glm::vec3 GetVertexNormalMap(glm::vec2 uv, const glm::vec3& worldTangent, const glm::vec3& worldBitangent, const glm::vec3& worldNormal) const override
    {
        assert(HasNormalMap());
        const Material* material = parentMesh->GetMaterial();
        Texture* normalTexture = material->GetTexture("normalTexture");
        glm::vec3 normalMap = glm::normalize(glm::vec3(normalTexture->Sample(uv)) * 2.f - 1.f);
        return glm::mat3(worldTangent, worldBitangent, worldNormal) * normalMap;
    }

    virtual glm::vec2 GetVertexUV(int index) const override
    {
        return uvs[index];
    }

    virtual glm::vec3 GetVertexTangent(int index) const override
    {
        return tangents[index];
    }

    virtual glm::vec3 GetVertexBitangent(int index) const override
    {
        return bitangents[index];
    }

protected:
    std::array<glm::vec3, N> positions;
    std::array<glm::vec3, N> normals;
    bool hasNormals;

    std::array<glm::vec2, N> uvs;
    bool hasUVs;
    
    std::array<glm::vec3, N> tangents;
    std::array<glm::vec3, N> bitangents;
    bool hasTangentBitangents;

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
