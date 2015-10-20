#include "common/Scene/Lights/Light.h"

glm::vec3 Light::GetLightColor() const
{
    return lightColor;
}

void Light::SetLightColor(glm::vec3 input)
{
    lightColor = input;
}