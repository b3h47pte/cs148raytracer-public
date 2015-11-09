#include "common/Rendering/Material/BlinnPhong/BlinnPhongMaterial.h"
#include "common/Intersection/IntersectionState.h"
#include "common/Scene/Lights/Light.h"
#include "common/Utility/Texture/TextureLoader.h"
#include "common/Rendering/Textures/Texture2D.h"
#include "assimp/material.h"

BlinnPhongMaterial::BlinnPhongMaterial():
    shininess(0.f)
{
}

void BlinnPhongMaterial::SetDiffuse(glm::vec3 input)
{
    diffuseColor = input;
}

void BlinnPhongMaterial::SetSpecular(glm::vec3 inputColor, float inputShininess)
{
    specularColor = inputColor;
    shininess = inputShininess;
}

glm::vec3 BlinnPhongMaterial::ComputeDiffuse(const IntersectionState& intersection, const glm::vec3& lightColor, const float NdL, const float NdH, const float NdV, const float VdH) const
{
    const glm::vec3 useDiffuseColor = (textureStorage.find("diffuseTexture") != textureStorage.end()) ? glm::vec3(textureStorage.at("diffuseTexture")->Sample(intersection.ComputeUV())) : diffuseColor;
    const float d = NdL;
    const glm::vec3 diffuseResponse = d * useDiffuseColor * lightColor;
    return diffuseResponse;
}

glm::vec3 BlinnPhongMaterial::ComputeSpecular(const IntersectionState& intersection, const glm::vec3& lightColor, const float NdL, const float NdH, const float NdV, const float VdH) const
{
    const glm::vec3 useSpecularColor = (textureStorage.find("specularTexture") != textureStorage.end()) ? glm::vec3(textureStorage.at("specularTexture")->Sample(intersection.ComputeUV())) : specularColor;
    const float highlight = std::pow(NdH, shininess);
    const glm::vec3 specularResponse = highlight * specularColor * lightColor;
    return specularResponse;
}

std::shared_ptr<Material> BlinnPhongMaterial::Clone() const
{
    return std::make_shared<BlinnPhongMaterial>(*this);
}

void BlinnPhongMaterial::LoadMaterialFromAssimp(std::shared_ptr<aiMaterial> assimpMaterial)
{
    if (!assimpMaterial) {
        return;
    }

    Material::LoadMaterialFromAssimp(assimpMaterial);

    assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, glm::value_ptr(diffuseColor), nullptr);
    assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, glm::value_ptr(specularColor), nullptr);
    assimpMaterial->Get(AI_MATKEY_SHININESS, &shininess, nullptr);
    if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE)) {
        aiString aiDiffusePath;
        assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiDiffusePath);
        std::string diffusePath(aiDiffusePath.C_Str());
        SetTexture("diffuseTexture", TextureLoader::LoadTexture(diffusePath));
    }

    if (assimpMaterial->GetTextureCount(aiTextureType_SPECULAR)) {
        aiString aiSpecularPath;
        assimpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &aiSpecularPath);
        std::string specularPath(aiSpecularPath.C_Str());
        SetTexture("specularTexture", TextureLoader::LoadTexture(specularPath));
    }

}

bool BlinnPhongMaterial::HasDiffuseReflection() const
{
    return (glm::length2(diffuseColor) > 0 || GetTexture("diffuseTexture"));
}

bool BlinnPhongMaterial::HasSpecularReflection() const
{
    return (glm::length2(specularColor) > 0 || GetTexture("specularTexture") || Material::HasSpecularReflection());
}

glm::vec3 BlinnPhongMaterial::GetBaseDiffuseReflection() const
{
    return diffuseColor;
}

glm::vec3 BlinnPhongMaterial::GetBaseSpecularReflection() const
{
    return glm::max(specularColor, Material::GetBaseSpecularReflection());
}