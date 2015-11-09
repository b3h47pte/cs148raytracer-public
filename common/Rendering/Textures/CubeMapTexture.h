#pragma once

#include "common/Rendering/Textures/Texture.h"

class CubeMapTexture : public Texture
{
public:
    // right, left, top, bottom, back, forward
    CubeMapTexture(unsigned char* data[6], int width, int height);
    virtual glm::vec4 Sample(const glm::vec2& coord) const override;
    virtual glm::vec4 Sample(const glm::vec3& coord) const override;
private:
    std::shared_ptr<class Texture2D> cubeTextures[6];
    int texWidth;
    int texHeight;
};