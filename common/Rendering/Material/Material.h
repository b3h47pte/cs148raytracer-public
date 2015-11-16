#pragma once

#include "common/common.h"

class Material: public std::enable_shared_from_this<Material>
{
public:
    Material();
    virtual ~Material();

    virtual glm::vec3 ComputeNonLightDependentBRDF(const class Renderer* renderer, const struct IntersectionState& intersection) const;
    virtual glm::vec3 ComputeBRDF(const struct IntersectionState& intersection, const glm::vec3& lightColor, const class Ray& toLightRay, const class Ray& fromCameraRay, float lightAttenuation, bool computeDiffuse = true, bool computeSpecular = true) const;
    
    virtual std::shared_ptr<Material> Clone() const = 0;
    virtual void LoadMaterialFromAssimp(std::shared_ptr<struct aiMaterial> assimpMaterial);

    virtual bool HasDiffuseReflection() const = 0;
    virtual bool HasSpecularReflection() const { return IsReflective(); }

    void SetReflectivity(float input);
    bool IsReflective() const { return reflectivity > SMALL_EPSILON; }

    void SetTransmittance(float input);
    bool IsTransmissive() const { return transmittance > SMALL_EPSILON; }
    float GetTransmittance() const { return transmittance; }

    virtual glm::vec3 GetBaseDiffuseReflection() const = 0;
    virtual glm::vec3 GetBaseSpecularReflection() const { return glm::vec3(reflectivity); }
    virtual glm::vec3 GetBaseTransmittance() const { return glm::vec3(transmittance); }

    void SetIOR(float input);
    float GetIOR() const { return indexOfRefraction; }

    void SetTexture(const std::string& id, std::shared_ptr<class Texture> inputTexture);
    class Texture* GetTexture(const std::string& id) const;

    void SetAmbient(const glm::vec3& input);

protected:
    virtual glm::vec3 ComputeDiffuse(const struct IntersectionState& intersection, const glm::vec3& lightColor, const float NdL, const float NdH, const float NdV, const float VdH) const;
    virtual glm::vec3 ComputeSpecular(const struct IntersectionState& intersection, const glm::vec3& lightColor, const float NdL, const float NdH, const float NdV, const float VdH) const;
    virtual glm::vec3 ComputeReflection(const class Renderer* renderer, const struct IntersectionState& intersection) const;
    virtual glm::vec3 ComputeTransmission(const class Renderer* renderer, const struct IntersectionState& intersection) const;

    std::unordered_map<std::string, std::shared_ptr<class Texture>> textureStorage;
private:
    glm::vec3 ambient;
    float reflectivity;         // Perfect reflection 
    float transmittance;        // Refraction
    float indexOfRefraction;
};
