#pragma once

#ifndef __MESH_LOADER__
#define __MESH_LOADER__

#include "common/common.h"

class MeshObject;
struct aiMaterial;
struct aiFace;
class PrimitiveBase;

namespace MeshLoader
{

std::vector<std::shared_ptr<MeshObject>> LoadMesh(const std::string& filename, std::vector<std::shared_ptr<aiMaterial>>* outputMaterials = nullptr);

void LoadFaceIntoPrimitive(const aiFace& face, PrimitiveBase& primitive, std::vector<glm::vec3>& allPosition, std::vector<glm::vec3>& allNormals, std::vector<glm::vec2>& allUV, std::vector<glm::vec3>& allTangents, std::vector<glm::vec3>& allBitangents);
void LoadFaceIntoPrimitive(unsigned int numVertices, unsigned int* indices, PrimitiveBase& primitive, std::vector<glm::vec3>& allPosition, std::vector<glm::vec3>& allNormals, std::vector<glm::vec2>& allUV, std::vector<glm::vec3>& allTangents, std::vector<glm::vec3>& allBitangents);
}

#endif
