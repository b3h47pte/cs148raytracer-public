#pragma once

#include "common/common.h"

class Material: public std::enable_shared_from_this<Material>
{
public:
    Material();
    virtual ~Material();

    virtual glm::vec3 ComputeNonLightDependentBRDF(const class Renderer* renderer, const struct IntersectionState& intersection) const;
    virtual glm::vec3 ComputeBRDF(const struct IntersectionState& intersection, const glm::vec3& lightColor, const class Ray& toLightRay, const class Ray& fromCameraRay, float lightAttenuation) const;
    
    virtual std::shared_ptr<Material> Clone() const = 0;
    virtual void LoadMaterialFromAssimp(std::shared_ptr<struct aiMaterial> assimpMaterial);

    void SetReflectivity(float input);
    bool IsReflective() const { return reflectivity > SMALL_EPSILON; }

    void SetTransmittance(float input);
    bool IsTransmissive() const { return transmittance > SMALL_EPSILON; }

    void SetIOR(float input);
    float GetIOR() const { return indexOfRefraction; }

    void SetTexture(const std::string& id, std::shared_ptr<class Texture> inputTexture);

protected:
    virtual glm::vec3 ComputeDiffuse(const struct IntersectionState& intersection, const glm::vec3& lightColor, const float NdL, const float NdH, const float NdV, const float VdH) const;
    virtual glm::vec3 ComputeSpecular(const struct IntersectionState& intersection, const glm::vec3& lightColor, const float NdL, const float NdH, const float NdV, const float VdH) const;
    virtual glm::vec3 ComputeReflection(const class Renderer* renderer, const struct IntersectionState& intersection) const;
    virtual glm::vec3 ComputeTransmission(const class Renderer* renderer, const struct IntersectionState& intersection) const;

    std::unordered_map<std::string, std::shared_ptr<class Texture>> textureStorage;
private:
    float reflectivity;         // Perfect reflection 
    float transmittance;        // Refraction
    float indexOfRefraction;
};
