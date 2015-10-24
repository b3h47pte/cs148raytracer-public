#include "common/Rendering/Material/BlinnPhong/BlinnPhongMaterial.h"
#include "common/Intersection/IntersectionState.h"
#include "common/Scene/Lights/Light.h"
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

glm::vec3 BlinnPhongMaterial::ComputeBRDF(const IntersectionState& intersection, const Light& relevantLight, const Ray& toLightRay, const Ray& fromCameraRay) const
{
    const glm::vec3 N = intersection.ComputeNormal();
    const glm::vec3 L = toLightRay.GetRayDirection();
    const glm::vec3 V = -1.f * fromCameraRay.GetRayDirection();
    const glm::vec3 H = glm::normalize(L + V);

    const float NdL = std::min(std::max(glm::dot(N, L), 0.f), 1.f);
    const float NdH = std::min(std::max(glm::dot(N, H), 0.f), 1.f);
    
    const glm::vec3 lightColor = relevantLight.GetLightColor();

    const float d = NdL;
    const glm::vec3 diffuseResponse = d * diffuseColor * lightColor;

    const float highlight = std::pow(NdH, shininess);
    const glm::vec3 specularResponse = highlight * specularColor * lightColor;

    return diffuseResponse + specularResponse;
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

    assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, glm::value_ptr(diffuseColor), nullptr);
    assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, glm::value_ptr(specularColor), nullptr);
    assimpMaterial->Get(AI_MATKEY_SHININESS, &shininess, nullptr);

    if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE)) {
        aiString aiDiffusePath;
        assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiDiffusePath);
        std::string diffusePath(aiDiffusePath.C_Str());
        (void)diffusePath;
    }

    if (assimpMaterial->GetTextureCount(aiTextureType_SPECULAR)) {
        aiString aiSpecularPath;
        assimpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &aiSpecularPath);
        std::string specularPath(aiSpecularPath.C_Str());
        (void)specularPath;
    }

}