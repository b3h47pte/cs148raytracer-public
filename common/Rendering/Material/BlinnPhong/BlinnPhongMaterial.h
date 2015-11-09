#pragma once

#include "common/Rendering/Material/Material.h"

class BlinnPhongMaterial : public Material
{
public:
    BlinnPhongMaterial();

    void SetDiffuse(glm::vec3 input);
    void SetSpecular(glm::vec3 inputColor, float inputShininess);

    virtual std::shared_ptr<Material> Clone() const override;

    virtual void LoadMaterialFromAssimp(std::shared_ptr<struct aiMaterial> assimpMaterial) override;

    virtual bool HasDiffuseReflection() const;
    virtual bool HasSpecularReflection() const;

    virtual glm::vec3 GetBaseDiffuseReflection() const;
    virtual glm::vec3 GetBaseSpecularReflection() const;
protected:
    virtual glm::vec3 ComputeDiffuse(const struct IntersectionState& intersection, const glm::vec3& lightColor, const float NdL, const float NdH, const float NdV, const float VdH) const override;
    virtual glm::vec3 ComputeSpecular(const struct IntersectionState& intersection, const glm::vec3& lightColor, const float NdL, const float NdH, const float NdV, const float VdH) const override;

private:
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
};
