#include "common/Rendering/Material/Material.h"
#include "common/Rendering/Renderer.h"
#include "common/Intersection/IntersectionState.h"
#include "common/Scene/Lights/Light.h"
#include "assimp/material.h"

Material::Material():
    reflectivity(0.f), transmittance(0.f), indexOfRefraction(1.f)
{
}

Material::~Material()
{
}

glm::vec3 Material::ComputeNonLightDependentBRDF(const class Renderer* renderer, const struct IntersectionState& intersection) const
{
    const glm::vec3 reflectionColor = ComputeReflection(renderer, intersection);
    const glm::vec3 transmissionColor = ComputeTransmission(renderer, intersection);
    return reflectivity * reflectionColor + transmittance * transmissionColor;
}

glm::vec3 Material::ComputeBRDF(const struct IntersectionState& intersection, const glm::vec3& lightColor, const class Ray& toLightRay, const class Ray& fromCameraRay, float lightAttenuation) const
{
    const glm::vec3 N = intersection.ComputeNormal();
    const glm::vec3 L = toLightRay.GetRayDirection();
    const glm::vec3 V = -1.f * fromCameraRay.GetRayDirection();
    const glm::vec3 H = glm::normalize(L + V);

    const float NdL = std::min(std::max(glm::dot(N, L), 0.f), 1.f);
    const float NdH = std::min(std::max(glm::dot(N, H), 0.f), 1.f);
    const float NdV = std::min(std::max(glm::dot(N, V), 0.f), 1.f);
    const float VdH = std::min(std::max(glm::dot(V, H), 0.f), 1.f);

    const glm::vec3 diffuseColor = ComputeDiffuse(intersection, lightColor, NdL, NdH, NdV, VdH);
    const glm::vec3 specularColor = ComputeSpecular(intersection, lightColor, NdL, NdH, NdV, VdH);

    const float attenuation = std::max((1.f - reflectivity - transmittance) * lightAttenuation, 0.f);
    return attenuation * (diffuseColor + specularColor);
}

glm::vec3 Material::ComputeDiffuse(const struct IntersectionState& intersection, const glm::vec3& lightColor, const float NdL, const float NdH, const float NdV, const float VdH) const
{
    return glm::vec3();
}

glm::vec3 Material::ComputeSpecular(const struct IntersectionState& intersection, const glm::vec3& lightColor, const float NdL, const float NdH, const float NdV, const float VdH) const
{
    return glm::vec3();
}

glm::vec3 Material::ComputeReflection(const class Renderer* renderer, const struct IntersectionState& intersection) const
{
    glm::vec3 reflectedColor;
    if (intersection.reflectionIntersection && intersection.reflectionIntersection->hasIntersection) {
        reflectedColor = renderer->ComputeSampleColor(*intersection.reflectionIntersection.get(), intersection.reflectionIntersection->intersectionRay);
    }
    return reflectedColor;
}

glm::vec3 Material::ComputeTransmission(const class Renderer* renderer, const struct IntersectionState& intersection) const
{
    glm::vec3 transmissionColor;
    if (intersection.refractionIntersection && intersection.refractionIntersection->hasIntersection) {
        transmissionColor = renderer->ComputeSampleColor(*intersection.refractionIntersection.get(), intersection.refractionIntersection->intersectionRay);
    }
    return transmissionColor;
}

void Material::SetReflectivity(float input)
{
    reflectivity = input;
}

void Material::SetTransmittance(float input)
{
    transmittance = input;
}

void Material::SetIOR(float input)
{
    indexOfRefraction = input;
}

void Material::LoadMaterialFromAssimp(std::shared_ptr<struct aiMaterial> assimpMaterial)
{
    if (!assimpMaterial) {
        return;
    }

    float opacity;
    assimpMaterial->Get(AI_MATKEY_OPACITY, &opacity, nullptr);
    transmittance = 1.f - opacity;

    assimpMaterial->Get(AI_MATKEY_REFRACTI, &indexOfRefraction, nullptr);
}

void Material::SetTexture(const std::string& id, std::shared_ptr<class Texture> inputTexture)
{
    textureStorage[id] = std::move(inputTexture);
}
