#pragma once

#include "common/Rendering/Textures/Texture.h"

class Texture2D : public Texture
{
public:
    Texture2D(unsigned char* rawData, int width, int height);
    virtual ~Texture2D();

    virtual glm::vec4 Sample(const glm::vec2& coord) const override;
    virtual glm::vec4 Sample(const glm::vec3& coord) const override;
private:
    glm::vec4 InternalSample(const glm::ivec2& coord) const;
    glm::ivec2 HandleBorderCondition(const glm::ivec2& coord) const;
    int ComputeLinearIndex(const glm::ivec2& pixel) const;

    unsigned char* textureData;
    int texWidth;
    int texHeight;
};