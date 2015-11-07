#include "common/Scene/Geometry/Mesh/MeshObject.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include "common/Scene/Geometry/Primitives/Triangle/Triangle.h"
#include "common/Scene/Geometry/Primitives/PrimitiveBase.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "common/Scene/Geometry/Primitives/Primitive.h"
#include <map>
#include <queue>

namespace MeshLoader
{

void LoadFaceIntoPrimitive(const aiFace& face, PrimitiveBase& primitive, std::vector<glm::vec3>& allPosition, std::vector<glm::vec3>& allNormals, std::vector<glm::vec2>& allUV, std::vector<glm::vec3>& allTangents, std::vector<glm::vec3>& allBitangents)
{
    LoadFaceIntoPrimitive(face.mNumIndices, face.mIndices, primitive, allPosition, allNormals, allUV, allTangents, allBitangents);
}

    void LoadFaceIntoPrimitive(unsigned int numVertices, unsigned int* indices, PrimitiveBase& primitive, std::vector<glm::vec3>& allPosition, std::vector<glm::vec3>& allNormals, std::vector<glm::vec2>& allUV, std::vector<glm::vec3>& allTangents, std::vector<glm::vec3>& allBitangents)
{
    assert(numVertices == primitive.GetTotalVertices());
    bool hasNormals = allNormals.size() == allPosition.size();
    bool hasUV = allUV.size() == allPosition.size();
    bool hasTangentsAndBitangents = (allTangents.size() == allPosition.size()) && (allBitangents.size() == allPosition.size());

    for (unsigned int i = 0; i < numVertices; ++i) {
        primitive.SetVertexPosition(i, allPosition[indices[i]]);

        if (hasNormals) {
            primitive.SetVertexNormal(i, allNormals[indices[i]]);
        }

        if (hasUV) {
            primitive.SetVertexUV(i, allUV[indices[i]]);
        }

        if (hasTangentsAndBitangents) {
            primitive.SetVertexTangentBitangent(i, allTangents[indices[i]], allBitangents[indices[i]]);
        }
    }
}

std::vector<std::shared_ptr<MeshObject>> LoadMesh(const std::string& filename, std::vector<std::shared_ptr<aiMaterial>>* outputMaterials)
{

#ifndef ASSET_PATH
    static_assert(false, "ASSET_PATH is not defined. Check to make sure your projects are setup correctly");
#endif

    Assimp::Importer importer;
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);

    const std::string completeFilename = std::string(STRINGIFY(ASSET_PATH)) + "/" + filename;

    const aiScene* scene = importer.ReadFile(completeFilename.c_str(),
            aiProcess_GenNormals |
            aiProcess_CalcTangentSpace       | 
            aiProcess_Triangulate            |
            aiProcess_JoinIdenticalVertices  |
            aiProcess_FixInfacingNormals |
            aiProcess_FindInstances |
            aiProcess_SortByPType);
    if (!scene) {
        std::cerr << "ERROR: Assimp failed -- " << importer.GetErrorString() << std::endl;
        return {};
    }

    std::vector<std::shared_ptr<aiMaterial>> sceneMaterials;
    if (outputMaterials) {
        sceneMaterials.resize(scene->mNumMaterials);
        for (unsigned int m = 0; m < scene->mNumMaterials; ++m) {
            aiMaterial* material = scene->mMaterials[m];
            std::shared_ptr<aiMaterial> dstMaterial = std::make_shared<aiMaterial>();
            aiMaterial::CopyPropertyList(dstMaterial.get(), material);
            sceneMaterials[m] = dstMaterial;
        }
    }

    std::vector<std::shared_ptr<MeshObject>> loadedMeshes;
    for (decltype(scene->mNumMeshes) i = 0; i < scene->mNumMeshes; ++i) {
        const aiMesh* mesh = scene->mMeshes[i];
        if (!mesh->HasPositions()) {
            std::cerr << "WARNING: A mesh in " << filename << " does not have positions. Skipping." << std::endl;
            continue;
        }
        std::shared_ptr<MeshObject> newMesh = std::make_shared<MeshObject>();
        auto totalVertices = mesh->mNumVertices;
        std::vector<glm::vec3> allPosition(totalVertices);
        std::vector<glm::vec3> allNormals;
        if (mesh->HasNormals()) {
            allNormals.resize(totalVertices);
        }

        std::vector<glm::vec2> allUV;
        if (mesh->HasTextureCoords(0)) {
            allUV.resize(totalVertices);
        }

        std::vector<glm::vec3> allTangents;
        std::vector<glm::vec3> allBitangents;
        if (mesh->HasTangentsAndBitangents()) {
            allTangents.resize(totalVertices);
            allBitangents.resize(totalVertices);
        }

        for (decltype(totalVertices) v = 0; v < totalVertices; ++v) {
            allPosition[v] = glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
            
            if (mesh->HasNormals()) {
                allNormals[v] = glm::vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
            }

            if (mesh->HasTextureCoords(0)) {
                allUV[v] = glm::vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
            }

            if (mesh->HasTangentsAndBitangents()) {
                allTangents[v] = glm::vec3(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z);
                allBitangents[v] = glm::vec3(mesh->mBitangents[v].x, mesh->mBitangents[v].y, mesh->mBitangents[v].z);
            }
        }

        if (mesh->HasFaces()) {
            for (decltype(mesh->mNumFaces) f = 0; f < mesh->mNumFaces; ++f) {
                const aiFace& face =  mesh->mFaces[f];
                std::shared_ptr<PrimitiveBase> newPrimitive = nullptr;
                if (face.mNumIndices == 3) {
                    newPrimitive = std::make_shared<Triangle>(newMesh.get());
                    LoadFaceIntoPrimitive(face, *newPrimitive.get(), allPosition, allNormals, allUV, allTangents, allBitangents);
                } else {
                    std::cerr << "WARNING: Input mesh has an unsupported primitive type. Skipping face with: " << face.mNumIndices << " vertices." << std::endl;
                    continue;
                }
                assert(newPrimitive);
                newMesh->AddPrimitive(newPrimitive);
            }
        } else {
            // Assume triangles
            assert(totalVertices % 3 == 0);
            for (decltype(totalVertices) v = 0; v < totalVertices; v += 3) {
                std::shared_ptr<PrimitiveBase> newPrimitive = std::make_shared<Triangle>(newMesh.get());
                unsigned int indices[3] = { v, v + 1, v + 2 };
                LoadFaceIntoPrimitive(3, indices, *newPrimitive.get(), allPosition, allNormals, allUV, allTangents, allBitangents);
                assert(newPrimitive);
                newMesh->AddPrimitive(newPrimitive);
            }
        }

        loadedMeshes.push_back(std::move(newMesh));
        if (outputMaterials) {
            outputMaterials->push_back(sceneMaterials[mesh->mMaterialIndex]);
        }
    }

    // Traverse nodes to set mesh names
    std::queue<aiNode*> nodes;
    nodes.push(scene->mRootNode);
    while (!nodes.empty()) {
        aiNode* currentNode = nodes.front();
        nodes.pop();

        if (currentNode->mNumMeshes) {
            for (unsigned int i = 0; i < currentNode->mNumMeshes; ++i) {
                loadedMeshes[currentNode->mMeshes[i]]->SetName(currentNode->mName.C_Str());
            }
        }

        for (unsigned int i = 0; i < currentNode->mNumChildren; ++i) {
            nodes.push(currentNode->mChildren[i]);
        }
    }

    return loadedMeshes;
}

}
